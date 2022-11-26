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
  _speed = {0.0f, 0.0f, 0.0f};
}

Player::~Player() { delete _camera; }

void Player::update(float ts) {
  freezeLogic();
  if (!_enabled)
    return;

  double xpos, ypos;
  glfwGetCursorPos(_window, &xpos, &ypos);

  auto scrnSize = _camera->getWindowSize();
  double centerX = glm::ceil(scrnSize.first / 2.0f);
  double centerY = glm::ceil(scrnSize.second / 2.0f);

  glfwSetCursorPos(_window, centerX, centerY);

  _horAng += MOUSE_SPEED * ts * (centerX - xpos);
  _vertAng += MOUSE_SPEED * ts * (centerY - ypos);

  if (_vertAng < -1.57f)
    _vertAng = -1.57f;
  else if (_vertAng > 1.57)
    _vertAng = 1.57;

  _direction = {cos(_vertAng) * sin(_horAng), sin(_vertAng),
                cos(_vertAng) * cos(_horAng)};

  glm::vec3 right =
      glm::vec3(sin(_horAng - 3.14f / 2.0f), 0, cos(_horAng - 3.14f / 2.0f));

  _up = glm::cross(right, _direction);

  glm::vec3 dir2d = {sin(_horAng), 0.f, cos(_horAng)};
  glm::vec3 acc = {0.0, 0.0, 0.0};
  if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS) {
    acc += dir2d * ts * PLAYER_SPEED;
  }
  // Move backward
  if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS) {
    acc -= dir2d * ts * PLAYER_SPEED;
  }
  // Strafe right
  if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS) {
    acc += right * ts * PLAYER_SPEED;
  }
  // Strafe left
  if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS) {
    acc -= right * ts * PLAYER_SPEED;
  }
  // Jump
  if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    if (_position.y == 0.f) {
      _position.y = 0.1f;
      _speed.y += ts * PLAYER_JUMP;
    }
  }
  acc.y -= GRAVITY;

  _speed += acc * ts - _speed * PLAYER_DRAG;
  _position += _speed * ts;

  // clip player to ground
  if (_position.y < 0.f)
    _position.y = 0.f;
}

void Player::freezeLogic() {
  if (_enabled) {
    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      _enabled = false;
      glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

  } else {
    if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
      _enabled = true;
      glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
  }
}

glm::mat4 Player::getPlayerProjection() {
  glm::vec3 camera_pos = {0.f, 1.8f, 0.f};
  camera_pos += _position;
  return _camera->getProjection(camera_pos, _direction, _up);
}

glm::vec3 Player::getPlayerCameraPosition() {
  glm::vec3 pos = {0.f, 1.8f, 0.f};
  pos += _position;
  return pos;
}

} // namespace Game
