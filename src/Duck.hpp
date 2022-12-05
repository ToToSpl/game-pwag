#pragma once

#include "GameObject.hpp"
#include "Player.hpp"

namespace Game {
class Duck {
private:
  enum State { CIRCLING, DYING, DEAD, ATTACK };

  GameObject* _handler;
  GameEntity* _entity;
  float _aliveTime = 0.f, _ts_ms;
  State _state = State::CIRCLING;
  Player* _player;
  glm::vec3 _position, _circlePoint;
  glm::quat _orientation;

private:
  float getPlayerDistance();
  void circleState();
  void attackState();
  void dyingState();
  void deadState();
  void positionUpdate();

public:
  Duck(GameObject* handler, Player* player);
  ~Duck();

public:
  void update(float ts_ms);
  bool attacked();
  bool shouldRemove();
};
} // namespace Game
