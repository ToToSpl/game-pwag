#pragma once

#include "GameObject.hpp"
#include "Player.hpp"
#include "constants.h"

namespace Game {
class Duck {
private:
  enum State { CIRCLING, DYING, DEAD, ATTACK, REMOVED };

  GameObject* _handler;
  GameEntity* _entity;
  float _aliveTime = 0.f, _ts_ms;
  float _seed = 0.f, _dyingStart = 0.f, _cooldown = 0.f;
  State _state = State::CIRCLING;
  Player* _player;
  glm::vec3 _position, _oldPosition, _circlePoint;
  glm::quat _orientation, _deathRoll;

private:
  float getPlayerDistance();
  void circleState();
  void attackState(std::vector<Duck*>& otherDucks);
  void dyingState();
  void deadState();
  void positionUpdate();

public:
  Duck(GameObject* handler, Player* player);
  ~Duck();

public:
  void update(float ts_ms, std::vector<Duck*>& otherDucks);
  bool attacked();
  bool shouldRemove();
  inline glm::vec3 getPosition() { return _position; };
};
} // namespace Game
