#include "Renderer.hpp"
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/OpenGL.h>
#include <cassert>
#include <chrono>
#include <iostream>

#define GLCall(x)                                                              \
  GLClearError();                                                              \
  x;                                                                           \
  assert(GLLogCall());

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
  /* We need to explicitly ask for a 3.2 context on OS X */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
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
