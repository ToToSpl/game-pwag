#pragma once
#include <string>

#include "../lib/json.hpp"
#include "Scene.hpp"

namespace Game {

using json = nlohmann::json;

class GameObject {
public:
  GameObject(Scene& scene, std::string configFile);
  ~GameObject();

private:
  Scene& _scene;
  std::string _configFile;
  json _data;

private:
  void loadObject();
};

} // namespace Game
