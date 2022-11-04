#include "Scene.hpp"

namespace Game {
Scene::Scene() {}
Scene::~Scene() {}

void Scene::addBoxEntity(json data) {
  std::string name = data["name"];
  float position[3], dimension[3];

  for (int i = 0; i < 3; i++) {
    position[i] = data["position"][i];
    dimension[i] = data["dimension"][i];
  }
}

} // namespace Game
