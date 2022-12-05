#include "Duck.hpp"
#include "GameObject.hpp"
#include "constants.h"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/transform.hpp"
#include <cstdlib>
#include <iostream>

namespace Game {
Duck::Duck(GameObject* handler, Player* player)
    : _handler(handler), _player(player) {
  float pointAngle = std::rand() * 3.14f;
  glm::vec4 pos(glm::vec3(DUCK_START_POSITION), 0.f);
  _position = glm::vec3(pos * glm::rotate(pointAngle, glm::vec3({0, 1, 0})));
  _circlePoint = _position - glm::vec3(DUCK_CIRCLE_VEC);
  _orientation = glm::angleAxis(-pointAngle + 1.57f, glm::vec3({0, 1, 0}));
  _entity = _handler->spawn(_position, _orientation);
}

Duck::~Duck() { _handler->remove(_entity); }

void Duck::update(float ts_ms) {
  _ts_ms = ts_ms;
  _aliveTime += _ts_ms;

  switch (_state) {
  case CIRCLING:
    circleState();
    break;
  case ATTACK:
    attackState();
    break;
  case DYING:
    dyingState();
    break;
  case DEAD:
    deadState();
    break;
  }

  positionUpdate();
}

void Duck::positionUpdate() {
  _handler->moveAndRotate(_entity, _position, _orientation);
}

void Duck::circleState() {

  auto circPos = glm::vec4(_position - _circlePoint, 0);
  auto mat = glm::rotate(3.14f * 0.001f * _ts_ms, glm::vec3({0, 1, 0}));
  circPos = mat * circPos;
  _orientation = glm::quat(mat) * _orientation;
  _position = glm::vec3(circPos) + _circlePoint;
  _position.y += 0.005f * glm::sin(_aliveTime * 0.008f + 1.f);

  if (getPlayerDistance() < DUCK_AGRO_DISTANCE && false) // disable for debug
    _state = State::ATTACK;
}

void Duck::attackState() {}
void Duck::dyingState() {}
void Duck::deadState() {}

bool Duck::shouldRemove() { return _state == State::DEAD; }

float Duck::getPlayerDistance() {
  glm::vec3 dist = _player->getPlayerCameraPosition() - _position;
  return glm::length(dist);
}

bool Duck::attacked() { return getPlayerDistance() <= DUCK_ATTACK_DISTANCE; }

} // namespace Game
