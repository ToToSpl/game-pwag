#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>

#include "../lib/json.hpp"
#include "Scene.hpp"

namespace Game {

using json = nlohmann::json;

struct GameEntity {
  bool exists;
  std::vector<std::pair<u_int32_t, SceneObject*>> objects;
};

class GameObject {
public:
  GameObject(Scene& scene, std::string configFile);
  ~GameObject();

public:
  GameEntity* spawn(glm::vec3 pos, glm::quat rot);
  void remove(GameEntity* ent);

private:
  Scene& _scene;
  std::string _configFile;
  json _data;
  std::vector<u_int32_t> _entitiesIDs;

private:
  void loadObject();
};

} // namespace Game
