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
      _scene.addBoxEntity(model);
    } else {
    }
  }
}

} // namespace Game
