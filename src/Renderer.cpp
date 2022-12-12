#include "Renderer.hpp"
#include "Scene.hpp"
#include "constants.h"
#include <OpenGL/OpenGL.h>
#include <cassert>
#include <chrono>
#include <iostream>

#define GLCall(x)                                                              \
  GLClearError();                                                              \
  x;                                                                           \
  assert(GLLogCall());

#define STD_SHADER_FRAG BUILD_TO_ROOT + "shaders/basic.frag"
#define STD_SHADER_VERT BUILD_TO_ROOT + "shaders/basic.vert"
#define WIREFRAME_SHADER_FRAG BUILD_TO_ROOT + "shaders/wireframe.frag"
#define WIREFRAME_SHADER_VERT BUILD_TO_ROOT + "shaders/wireframe.vert"
#define POSTEFFECT_SHADER_FRAG BUILD_TO_ROOT + "shaders/posteffect.frag"
#define POSTEFFECT_SHADER_VERT BUILD_TO_ROOT + "shaders/posteffect.vert"
static void GLClearError() {
  while (glGetError() != GL_NO_ERROR)
    ;
}

static bool GLLogCall() {
  while (GLenum error = glGetError()) {
    std::cout << "[OpenGL Error] ( " << error << " )" << std::endl;
    return false;
  }
  return true;
}

namespace Game {
Renderer::Renderer(Scene& scene)
    : _scene(scene){

      };

Renderer::~Renderer() {
  if (_running)
    glfwTerminate();
};

bool Renderer::init(std::string window_name) {
  if (!glfwInit()) {
    std::cout << "Failed to init glfw!" << std::endl;
    return false;
  }
  _running = true;

#ifdef __APPLE__
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

  _window = glfwCreateWindow(1280, 720, window_name.c_str(), NULL, NULL);
  if (!_window) {
    glfwTerminate();
    std::cout << "Failed to create window!" << std::endl;
    return false;
  }

  glfwMakeContextCurrent(_window);

  if (glewInit() != GLEW_OK) {
    std::cout << "Failed to init glew!" << std::endl;
    return false;
  }

  GLCall(glGenVertexArrays(1, &_VertexArrayID));
  GLCall(glBindVertexArray(_VertexArrayID));

  _stdShaderProg = compileProgram(STD_SHADER_VERT, STD_SHADER_FRAG);
  _wireframeProg = compileProgram(WIREFRAME_SHADER_VERT, WIREFRAME_SHADER_FRAG);
  _postEffectProg =
      compileProgram(POSTEFFECT_SHADER_VERT, POSTEFFECT_SHADER_FRAG);

  _currentProgID = _stdShaderProg->programId;
  _cameraID = glGetUniformLocation(_stdShaderProg->programId, "MVP");
  _transformationID = glGetUniformLocation(_stdShaderProg->programId, "M");
  _normalMatID = glGetUniformLocation(_stdShaderProg->programId, "NormalMat");
  _cameraPosID = glGetUniformLocation(_stdShaderProg->programId, "V");
  _aliveID = glGetUniformLocation(_stdShaderProg->programId, "alive");

  _lightDirID = glGetUniformLocation(_stdShaderProg->programId, "lightDir");
  _lightPointID = glGetUniformLocation(_stdShaderProg->programId, "lightPoint");

  _healthID = glGetUniformLocation(_postEffectProg->programId, "health");
  _tsID = glGetUniformLocation(_postEffectProg->programId, "t_elapsed");

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // create plane for posteffects
  {
    float vertecies[4 * 2] = {-1.f, 1.f, 1.f, 1.f, -1.f, -1.f, 1.f, -1.f};
    glGenBuffers(1, &_postEffectVertArr);
    glBindBuffer(GL_ARRAY_BUFFER, _postEffectVertArr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 2, vertecies,
                 GL_STATIC_DRAW);

    u_int16_t indecies[6] = {0, 2, 1, 2, 3, 1};
    glGenBuffers(1, &_postEffectIndArr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _postEffectIndArr);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u_int16_t) * 6, indecies,
                 GL_STATIC_DRAW);
  }

  return true;
}

void Renderer::addDirectionalLight(glm::vec3 dir, float brightness) {
  auto d = glm::normalize(dir) * brightness;
  _lightDir = d;
}

void Renderer::addPointLight(glm::vec3 point) { _lightPoint = point; }

void Renderer::setupLight() {
  glUniform3fv(_lightDirID, 1, &_lightDir[0]);
  glUniform3fv(_lightPointID, 1, &_lightPoint[0]);
}

float Renderer::renderFrame(float ts) {
  _timeElapsed += ts;
  auto start = std::chrono::high_resolution_clock::now();

  _player->update(ts);
  glm::mat4 camera = _player->getPlayerProjection();
  glm::vec3 cameraPos = _player->getPlayerCameraPosition();

  /* Render here */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers

  if (_player->getWireframe() == false) {
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glUseProgram(_stdShaderProg->programId);
    if (_currentProgID != _stdShaderProg->programId) {
      _currentProgID = _stdShaderProg->programId;
      _cameraID = glGetUniformLocation(_stdShaderProg->programId, "MVP");
      _transformationID = glGetUniformLocation(_stdShaderProg->programId, "M");
      _normalMatID =
          glGetUniformLocation(_stdShaderProg->programId, "NormalMat");
      _cameraPosID = glGetUniformLocation(_stdShaderProg->programId, "V");
      _aliveID = glGetUniformLocation(_stdShaderProg->programId, "alive");
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    setupLight();

    std::vector<BasicEntity>* ents = _scene.getEntities();
    for (u_int32_t i = 0; i < ents->size(); i++) {
      _scene.renderEntityObjects(ts, (*ents)[i], _cameraID, camera, cameraPos,
                                 _transformationID, _normalMatID, _cameraPosID,
                                 _aliveID);
    }
  } else {

    glClearColor(1.f, 1.f, 1.f, 0.0f);
    glUseProgram(_wireframeProg->programId);
    if (_currentProgID != _wireframeProg->programId) {
      _currentProgID = _wireframeProg->programId;
      _cameraID = glGetUniformLocation(_wireframeProg->programId, "MVP");
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    std::vector<BasicEntity>* ents = _scene.getEntities();
    for (u_int32_t i = 0; i < ents->size(); i++) {
      _scene.renderEntityObjects(ts, (*ents)[i], _cameraID, camera, cameraPos,
                                 _transformationID, _normalMatID, _cameraPosID,
                                 _aliveID);
    }
  }

  // post effects
  {
    glUseProgram(_postEffectProg->programId);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _postEffectVertArr);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _postEffectIndArr);
    glUniform1f(_healthID, _player->getHealth());
    glUniform1f(_tsID, _timeElapsed);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)0);
    glDisableVertexAttribArray(0);
  }

  /* Swap front and back buffers */
  glfwSwapBuffers(_window);

  /* Poll for and process events */
  glfwPollEvents();

  auto end = std::chrono::high_resolution_clock::now();
  _lastFrame =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();
  return _lastFrame;
}

} // namespace Game
