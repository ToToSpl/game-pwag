#include "Player.hpp"
#include "constants.h"
#include <glm/gtx/transform.hpp>
#include <iostream>

namespace Game {

Player::Player(glm::vec3 startPos, double horAngle, double vertAngle,
               GLFWwindow* window)
    : _position(startPos), _horAng(horAngle), _vertAng(vertAngle),
      _window(window) {
  int w, h;
  glfwGetWindowSize(_window, &w, &h);
  _camera = new Camera(_window, w, h, true);
  _speed = {0.0f, 0.0f, 0.0f};
  _cutDir = {0, 0};
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

  if (!_mousePressed) {
    _horAng += MOUSE_SPEED * ts * (centerX - xpos);
    _vertAng += MOUSE_SPEED * ts * (centerY - ypos);
  } else {
    glm::vec2 dir({centerX - xpos, centerY - ypos});
    if (dir.x != 0 && dir.y != 0)
      _cutDir = dir;
  }

  if (_vertAng < -1.57f)
    _vertAng = -1.57f;
  else if (_vertAng > 1.57)
    _vertAng = 1.57;

  _direction = {cos(_vertAng) * sin(_horAng), sin(_vertAng),
                cos(_vertAng) * cos(_horAng)};

  _right =
      glm::vec3(sin(_horAng - 3.14f / 2.0f), 0, cos(_horAng - 3.14f / 2.0f));

  _up = glm::cross(_right, _direction);

  glm::vec3 dir2d = {sin(_horAng), 0.f, cos(_horAng)};
  glm::vec3 acc = {0.0, 0.0, 0.0};
  if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
    acc += dir2d;
  // Move backward
  if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
    acc -= dir2d;
  // Strafe right
  if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
    acc += _right;
  // Strafe left
  if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
    acc -= _right;

  // check additional
  if (glfwGetKey(_window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
    if (!_wireframePressed) {
      _wireframePressed = true;
      _wireframe = !_wireframe;
    }
  } else
    _wireframePressed = false;

  if (glfwGetMouseButton(_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
    _mousePressed = true;
    _mousePressedFrame++;
  } else if (_mousePressed) {
    if (_cutDir.x != 0 && _cutDir.y != 0) {
      _cutDir = glm::normalize(_cutDir);
      // do stuff with cutDir
    }

    _mousePressed = false;
    _mousePressedFrame = 0;
    _cutDir = glm::vec2({0, 0});
  }

  if (glm::length(acc) != 0.0f)
    acc = PLAYER_ACC * ts * glm::normalize(acc);

  _speed += acc * ts;
  for (u_int32_t i = 0; i < ts; i++)
    _speed *= PLAYER_DRAG;

  if (glm::length(_speed) > PLAYER_SPEED)
    _speed = glm::normalize(_speed) * PLAYER_SPEED;
  _position += _speed * ts;

  // clip player to ground
  if (_position.y != 0.f)
    _position.y = 0.f;

  placeKatana(ts);
  _gameTime += ts;
}

void Player::placeKatana(float ts_ms) {
  glm::vec3 offset(KATANA_POS_REL);
  if (_mousePressed) {
    float step = _mousePressedFrame * 0.002;
    offset.y += step > 0.05f ? 0.05f : step;

  } else {
    offset.y += 0.01f * sin(_gameTime * 0.0015);
  }
  glm::mat4 rot = glm::rotate(0.0f, glm::vec3({0, 0, 1}));
  _katanaObj->attachTo(_katanaEnt, _position + glm::vec3(CAMERA_POS_REL),
                       _direction, offset, rot);
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
  return _camera->getProjection(_position + glm::vec3(CAMERA_POS_REL),
                                _direction, _up);
}

glm::vec3 Player::getPlayerCameraPosition() {
  glm::vec3 pos = {0.f, 1.8f, 0.f};
  pos += _position;
  return pos;
}

void Player::addKatana(GameObject& katana) {
  _katanaObj = &katana;
  _katanaEnt = katana.spawn({0, 0, 0}, {1, 0, 0, 0});
}

} // namespace Game
