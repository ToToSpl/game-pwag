#include "Player.hpp"
#include "constants.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Game {
// ------ Camera --------
Camera::Camera(GLFWwindow* window, int width, int height, bool resize)
    : _window(window), _width(width), _height(height), _resize(resize) {
  _fov = FOV;
  _projection = glm::perspective(glm::radians(_fov),
                                 (float)_width / (float)_height, 0.1f, 300.0f);
}
Camera::~Camera(){};

glm::mat4 Camera::getProjection(glm::vec3 position, glm::vec3 direction) {
  if (_resize) {
    int nW, nH;
    glfwGetWindowSize(_window, &nW, &nH);
    if (nW != _width || nH != _height) {
      _width = nW;
      _height = nH;
      _projection = glm::perspective(
          glm::radians(_fov), (float)_width / (float)_height, 0.1f, 300.0f);
    }
  }

  glm::mat4 view =
      glm::lookAt(position, position + direction, glm::vec3(0, 1, 0));

  return _projection * view;
}

} // namespace Game
