#include "Player.hpp"
#include "constants.h"

namespace Game {

Player::Player(glm::vec3 startPos, double horAngle, double vertAngle,
               GLFWwindow* window)
    : _position(startPos), _horAng(horAngle), _vertAng(vertAngle),
      _window(window) {
  int w, h;
  glfwGetWindowSize(_window, &w, &h);
  _camera = new Camera(_window, w, h, true);
}

Player::~Player() { delete _camera; }

void Player::update(float ts) {
  double xpos, ypos;
  glfwGetCursorPos(_window, &xpos, &ypos);

  auto scrnSize = _camera->getWindowSize();
  double centerX = scrnSize.first / 2.0f;
  double centerY = scrnSize.second / 2.0f;

  glfwSetCursorPos(_window, centerX, centerY);

  _horAng += MOUSE_SPEED * ts * (centerX - xpos);
  _vertAng += MOUSE_SPEED * ts * (centerY - ypos);

  _direction = {cos(_vertAng) * sin(_horAng), sin(_vertAng),
                cos(_vertAng) * cos(_horAng)};

  glm::vec3 right =
      glm::vec3(sin(_horAng - 3.14f / 2.0f), 0, cos(_horAng - 3.14f / 2.0f));

  _up = glm::cross(right, _direction);

  if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS) {
    _position += _direction * ts * PLAYER_SPEED;
  }
  // Move backward
  if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS) {
    _position -= _direction * ts * PLAYER_SPEED;
  }
  // Strafe right
  if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS) {
    _position += right * ts * PLAYER_SPEED;
  }
  // Strafe left
  if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS) {
    _position -= right * ts * PLAYER_SPEED;
  }

  // clip player to gound
  _position.y = 0.0;
}

glm::mat4 Player::getPlayerProjection() {
  return _camera->getProjection(_position, _direction, _up);
}

} // namespace Game
