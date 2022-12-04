#include "GameObject.hpp"
#include <fstream>
#include <glm/gtx/transform.hpp>
#include <iostream>

namespace Game {

GameObject::GameObject(Scene& scene, std::string configfile)
    : _configFile(configfile), _scene(scene) {
  loadObject();
}

GameObject::~GameObject() {}

void GameObject::loadObject() {
  _data = json::parse(std::fstream(_configFile));

  for (auto model : _data["shape"]) {
    std::string type = model["type"];

    if (type == "box") {
      u_int32_t id = _scene.addBoxEntity(model, _configFile);
      _entitiesIDs.push_back(id);
    } else if (type == "obj") {
      u_int32_t id = _scene.addObjEntity(model, _configFile);
      _entitiesIDs.push_back(id);
    } else {
    }
  }
}

GameEntity* GameObject::spawn(glm::vec3 pos, glm::quat rot) {
  GameEntity* ent = new GameEntity;
  for (u_int32_t i = 0; i < _entitiesIDs.size(); i++)
    ent->objects.push_back(
        {_entitiesIDs[i], _scene.spawnEntity(_entitiesIDs[i], pos, rot)});
  return ent;
}

void GameObject::remove(GameEntity* ent) {
  for (u_int32_t i = 0; i < ent->objects.size(); i++)
    _scene.removeEntity(ent->objects[i].first, ent->objects[i].second);
  delete ent;
}

void GameObject::translate(GameEntity* ent, glm::vec3 vec) {
  for (u_int32_t i = 0; i < ent->objects.size(); i++) {
    auto obj = ent->objects[i].second;
    obj->transform = glm::translate(obj->transform, vec);
    obj->moved = true;
  }
}

void GameObject::moveTo(GameEntity* ent, glm::vec3 vec) {
  for (u_int32_t i = 0; i < ent->objects.size(); i++) {
    auto obj = ent->objects[i].second;
    glm::vec3 pos(obj->transform[3]);
    obj->transform = glm::translate(obj->transform, pos - vec);
    obj->moved = true;
  }
}

void GameObject::rotate(GameEntity* ent, glm::quat rot) {
  for (u_int32_t i = 0; i < ent->objects.size(); i++) {
    auto obj = ent->objects[i].second;
    glm::vec3 pos(obj->transform[3]);
    glm::mat4 rotM = glm::toMat4(rot);
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), pos);
    obj->transform = trans * rotM;
    obj->moved = true;
  }
}

void GameObject::attachTo(GameEntity* ent, glm::vec3 pos, glm::vec3 dir,
                          glm::vec3 offset, glm::mat4 init_rot) {

  glm::vec3 R = glm::normalize(glm::cross({0, 1, 0}, dir));
  glm::vec3 U = glm::normalize(glm::cross(dir, R));
  float dotYaw = glm::dot(R, {1, 0, 0});
  float angleYaw = glm::acos(dotYaw);
  if (R.z > 0.f)
    angleYaw *= -1.f;

  float dotPitch = glm::dot(U, {0, 1, 0});
  float anglePitch = glm::acos(dotPitch);
  if (glm::dot(dir, {0, 1, 0}) > 0.f)
    anglePitch *= -1.f;

  glm::mat4 rotYaw = glm::rotate(angleYaw, glm::vec3({0, 1, 0}));
  glm::mat4 rotPitch = glm::rotate(anglePitch, glm::vec3({1, 0, 0}));

  glm::mat4 view = glm::translate(glm::mat4(1), pos) * rotYaw * rotPitch *
                   glm::translate(glm::mat4(1), offset) * init_rot;

  for (u_int32_t i = 0; i < ent->objects.size(); i++) {
    auto obj = ent->objects[i].second;
    obj->transform = view;
    obj->moved = true;
  }
}

} // namespace Game
