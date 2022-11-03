#pragma once

#include "../lib/json.hpp"

namespace Game {
namespace Shape {

using json = nlohmann::json;

class Box {
public:
  Box(json& data);
  ~Box();

private:
  void loadShape();

private:
  json& _data;
  std::string _name;
  float _position[3];
  float _dimension[3];
};

} // namespace Shape
} // namespace Game
