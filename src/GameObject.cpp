#include "GameObject.hpp"
#include <fstream>
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

void GameObject::spawn(glm::vec3 pos, glm::quat rot) {
  for (u_int32_t i = 0; i < _entitiesIDs.size(); i++) {
    _scene.spawnEntity(_entitiesIDs[i], pos, rot);
  }
}

} // namespace Game
