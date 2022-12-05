#pragma once

#include "GameObject.hpp"

namespace Game {
class Duck {
private:
  GameObject _handler;
  GameEntity* _entity;

public:
  Duck(GameObject handler);
  ~Duck(){};

public:
  void update(float ts_ms);
};
} // namespace Game
