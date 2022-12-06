#include "Duck.hpp"
#include "GameObject.hpp"
#include "constants.h"
#include "glm/gtc/random.hpp"
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
  // _orientation = glm::angleAxis(pointAngle + 1.57f, glm::vec3({0, 1, 0}));
  _entity = _handler->spawn(_position, _orientation);
  _oldPosition = _position;
  _seed = glm::linearRand(0.f, 1.f);
  _deathRoll = glm::quat({1, 0, 0, 0});
}

Duck::~Duck() {}

void Duck::update(float ts_ms) {
  _ts_ms = ts_ms;
  _aliveTime += _ts_ms;
  if (_cooldown > 0.f)
    _cooldown -= 0.001f * _ts_ms;

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
    return;
  case REMOVED:
    return;
  }

  positionUpdate();
}

void Duck::positionUpdate() {
  glm::vec3 dx = _position - _oldPosition;
  float yaw = glm::atan(dx.x, dx.z);
  float pitch = -glm::atan(dx.y, glm::length(glm::vec2({dx.z, dx.x})));

  auto matRot = glm::rotate(yaw, glm::vec3({0, 1, 0})) *
                glm::rotate(pitch, glm::vec3({1, 0, 0}));

  _orientation = glm::quat(matRot) * _deathRoll;
  _handler->moveAndRotate(_entity, _position, _orientation);
  _oldPosition = _position;
}

void Duck::circleState() {

  auto circPos = glm::vec4(_position - _circlePoint, 0);
  auto mat = glm::rotate(3.14f * 0.001f * _ts_ms, glm::vec3({0, 1, 0}));
  circPos = mat * circPos;
  _orientation = glm::quat(mat) * _orientation;
  _position = glm::vec3(circPos) + _circlePoint;
  _position.y += 0.01f * glm::sin(_aliveTime * 0.008f + _seed);

  if (getPlayerDistance() < DUCK_AGRO_DISTANCE)
    _state = State::ATTACK;
}

void Duck::attackState() {
  auto step = glm::normalize(_player->getPlayerCameraPosition() - _position);

  step *= 2.f * 0.001f * _ts_ms;
  glm::vec3 perp({step.z, 0.f, -step.x});
  if (getPlayerDistance() > 1.f) {
    _position += step;
    _position.y += 0.01f * glm::sin(_aliveTime * 0.008f + _seed);
    _position += 0.3f * glm::sin(_aliveTime * 0.005f + _seed) * perp;

  } else
    _position += 0.001f * step;

  float playerDist = getPlayerDistance();
  if (playerDist < DUCK_ATTACK_DISTANCE) {
    if (_cooldown <= 0.f) {
      _player->hit();
      _cooldown = DUCK_COOLDOWN_TIME;
    }
  }
  if (playerDist < PLAYER_ATTACK_RANGE) {
    bool hit = _player->checkHit(_position);
    if (hit)
      _state = State::DYING;
  }
}
void Duck::dyingState() {
  if (_dyingStart == 0.f) {
    _dyingStart = _aliveTime;
    _handler->kill(_entity);
  }
  float dt = 0.001f * (_aliveTime - _dyingStart);
  float roll = 1.57f * dt;
  _deathRoll = glm::quat(glm::rotate(roll, glm::vec3({0, 0, 1})));
  if (dt >= 1.0f)
    _state = State::DEAD;
}
void Duck::deadState() {
  _handler->remove(_entity);
  _state = State::REMOVED;
}

bool Duck::shouldRemove() { return _state == State::REMOVED; }

float Duck::getPlayerDistance() {
  glm::vec3 dist = _player->getPlayerCameraPosition() - _position;
  return glm::length(dist);
}

bool Duck::attacked() { return getPlayerDistance() <= DUCK_ATTACK_DISTANCE; }

} // namespace Game
