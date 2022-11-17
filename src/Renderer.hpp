#pragma once
#include <GLFW/glfw3.h>
#include <string>

namespace Game {

class Renderer {
private:
  GLFWwindow* _window;
  bool _running = false;

public:
  Renderer();
  ~Renderer();

public:
  bool init(std::string window_name);
  inline bool shouldRun() { return !glfwWindowShouldClose(_window); };
  float renderFrame();
};
} // namespace Game
