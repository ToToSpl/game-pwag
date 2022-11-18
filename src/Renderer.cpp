#include "Renderer.hpp"
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

// oh boy, I do love putting everything in one file

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
Renderer::Renderer(){

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

  glGenVertexArrays(1, &_VertexArrayID);
  glBindVertexArray(_VertexArrayID);

  glfwMakeContextCurrent(_window);

  _stdShaderFrag = compileShader(STD_SHADER_FRAG);
  _stdShaderVert = compileShader(STD_SHADER_VERT);
  _stdShaderProg = compileProgram(_stdShaderVert, _stdShaderFrag);

  return true;
}

float Renderer::renderFrame() {
  auto start = std::chrono::high_resolution_clock::now();

  /* Render here */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers

  /* Swap front and back buffers */
  glfwSwapBuffers(_window);

  /* Poll for and process events */
  glfwPollEvents();

  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
      .count();
}

} // namespace Game
