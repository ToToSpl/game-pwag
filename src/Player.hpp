#pragma once
#include "Camera.hpp"
#include "constants.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Game {

class AcceleretionEngine {
private:
  float _spd = 0.0f;
  float _max_spd = 1.0f;
  float _drag = 0.0f;

public:
  AcceleretionEngine(float drag, float max_spd);
  ~AcceleretionEngine(){};

public:
  float getSpeed(float acc, float ts);
};

class Player {
private:
  Camera* _camera;
  glm::vec3 _position, _up, _direction;
  GLFWwindow* _window;
  double _horAng, _vertAng;
  bool _enabled = false;
  AcceleretionEngine _accX, _accY, _accZ, _accHor, _accVert;

private:
  void freezeLogic();

public:
  Player(glm::vec3 startPos, double horAngle, double vertAngle,
         GLFWwindow* window);
  ~Player();

public:
  void update(float ts);
  glm::mat4 getPlayerProjection();
};
} // namespace Game
