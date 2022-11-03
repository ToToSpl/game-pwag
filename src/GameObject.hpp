#pragma once
#include <string>

#include "../lib/json.hpp"
#include "Box.hpp"

namespace Game {

using json = nlohmann::json;

class GameObject {
public:
  GameObject(std::string configFile);
  ~GameObject();

private:
  std::string _configFile;
  json _data;
  std::vector<Shape::Box> _boxes;

private:
  void loadShape();
};

} // namespace Game
