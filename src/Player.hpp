#pragma once
#include "Camera.hpp"
#include "constants.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Game {

class Player {
private:
  Camera* _camera;
  glm::vec3 _position, _up, _direction, _speed;
  GLFWwindow* _window;
  double _horAng, _vertAng;
  bool _enabled = false;

private:
  void freezeLogic();

public:
  Player(glm::vec3 startPos, double horAngle, double vertAngle,
         GLFWwindow* window);
  ~Player();

public:
  void update(float ts);
  glm::mat4 getPlayerProjection();
  glm::vec3 getPlayerCameraPosition();
};
} // namespace Game
