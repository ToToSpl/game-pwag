#include "Box.hpp"

#include <iostream>

namespace Game {
namespace Shape {

Box::Box(json& data) : _data(data) { loadShape(); };
Box::~Box(){};

void Box::loadShape() {
  _name = _data["name"];

  for (int i = 0; i < 3; i++) {
    _position[i] = _data["position"][i];
    _dimension[i] = _data["dimension"][i];
  }
}

} // namespace Shape
} // namespace Game
