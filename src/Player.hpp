#pragma once
#include "Camera.hpp"
#include "GameObject.hpp"
#include "constants.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Game {

class Player {
private:
  Camera* _camera;
  glm::vec3 _position, _up, _right, _direction, _speed;
  glm::vec3 _cutDir;
  GLFWwindow* _window;
  float _gameTime = 0.f;
  float _animationRate = 0.f;
  glm::mat4 _rotMat1, _rotMat2;
  double _horAng, _vertAng;
  bool _enabled = false;
  bool _wireframe = false, _wireframePressed = false;
  bool _mousePressed = false;
  u_int32_t _mousePressedFrame = 0;
  GameEntity* _katanaEnt = nullptr;
  GameObject* _katanaObj = nullptr;

private:
  void freezeLogic();
  void placeKatana(float ts_ms);

public:
  Player(glm::vec3 startPos, double horAngle, double vertAngle,
         GLFWwindow* window);
  ~Player();

public:
  void update(float ts);
  glm::mat4 getPlayerProjection();
  glm::vec3 getPlayerCameraPosition();
  void addKatana(GameObject& katana);
  inline bool getWireframe() { return _wireframe; };
  inline bool getMousePressed() { return _mousePressed; };
};
} // namespace Game
