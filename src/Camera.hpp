#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Game {
class Camera {
private:
  int _width, _height;
  float _fov;
  bool _resize;
  glm::mat4 _projection;
  GLFWwindow* _window;

public:
  Camera(GLFWwindow* window, int width, int height, bool resize);
  ~Camera();

public:
  glm::mat4 getProjection(glm::vec3 position, glm::vec3 direction,
                          glm::vec3 up);
  std::pair<double, double> getWindowSize();

private:
};
} // namespace Game
