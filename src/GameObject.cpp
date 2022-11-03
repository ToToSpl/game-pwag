#include "GameObject.hpp"
#include <fstream>
#include <iostream>

namespace Game {

GameObject::GameObject(std::string configfile) {
  _configFile = configfile;
  loadShape();
}

GameObject::~GameObject() {}

void GameObject::loadShape() {
  _data = json::parse(std::fstream(_configFile));

  for (auto model : _data["shape"]) {
    std::string type = model["type"];

    if (type == "box") {
      _boxes.push_back(Shape::Box(model));
    } else {
    }
  }
}

} // namespace Game
