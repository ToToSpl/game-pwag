#include "Renderer.hpp"
#include "Scene.hpp"
#include "constants.h"
#include <cassert>
#include <chrono>
#include <iostream>

#define GLCall(x)                                                              \
  GLClearError();                                                              \
  x;                                                                           \
  assert(GLLogCall());

#define STD_SHADER_FRAG BUILD_TO_ROOT + "shaders/basic.frag"
#define STD_SHADER_VERT BUILD_TO_ROOT + "shaders/basic.vert"

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

  _stdShaderFrag = compileShader(STD_SHADER_FRAG);
  _stdShaderVert = compileShader(STD_SHADER_VERT);
  _stdShaderProg = compileProgram(_stdShaderVert, _stdShaderFrag);
  _cameraID = glGetUniformLocation(_stdShaderProg->programId, "MVP");
  _transformationID = glGetUniformLocation(_stdShaderProg->programId, "M");
  _normalMatID = glGetUniformLocation(_stdShaderProg->programId, "NormalMat");
  _cameraPosID = glGetUniformLocation(_stdShaderProg->programId, "V");

  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);

  return true;
}

float Renderer::renderFrame(float ts) {
  auto start = std::chrono::high_resolution_clock::now();

  _player->update(ts);
  glm::mat4 camera = _player->getPlayerProjection();
  glm::vec3 cameraPos = _player->getPlayerCameraPosition();

  /* Render here */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers

  glUseProgram(_stdShaderProg->programId);

  std::vector<BasicEntity>* ents = _scene.getEntities();
  for (u_int32_t i = 0; i < ents->size(); i++) {
    _scene.renderEntityObjects((*ents)[i], _cameraID, camera, cameraPos,
                               _transformationID, _normalMatID, _cameraPosID);
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
