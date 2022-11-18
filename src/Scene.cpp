#include "Scene.hpp"

namespace Game {
Scene::Scene() {}
Scene::~Scene() {} // if only one scene is used, we don't need to clear memory

u_int32_t Scene::addBoxEntity(json data) {
  std::string name = data["name"];
  float position[3], dimension[3];

  for (int i = 0; i < 3; i++) {
    position[i] = (float)data["position"][i];
    dimension[i] = 0.5f * (float)data["dimension"][i];
  }

  // we need to create eight vertecies
  // malloc, so we can pass it to entity later
  float* vertecies = (float*)malloc(sizeof(float) * 8 * 3);
  u_int8_t* indecies = (u_int8_t*)malloc(sizeof(u_int8_t) * 3 * 2 * 6);

  // clang-format off
  vertecies = (float[8 * 3]){
    // x y z
    position[0] - dimension[0], position[1] - dimension[1], position[2] - dimension[2], // left front bottom
    position[0] + dimension[0], position[1] - dimension[1], position[2] - dimension[2], // right front bottom
    position[0] - dimension[0], position[1] + dimension[1], position[2] - dimension[2], // left back bottom
    position[0] + dimension[0], position[1] + dimension[1], position[2] - dimension[2], // right back bottom
    position[0] - dimension[0], position[1] - dimension[1], position[2] + dimension[2], // left front top
    position[0] + dimension[0], position[1] - dimension[1], position[2] + dimension[2], // right front top
    position[0] - dimension[0], position[1] + dimension[1], position[2] + dimension[2], // left back top
    position[0] + dimension[0], position[1] + dimension[1], position[2] + dimension[2], // right back top
  };

  indecies = (u_int8_t[3 * 2 * 6]){
    0, 2, 1, // bottom 1
    3, 1, 2, // bottom 2
    4, 5, 6, // top 1
    7, 6, 5, // top 2
    0, 1, 4, // front 1
    5, 4, 1, // front 2
    3, 2, 7, // back 1
    6, 7, 2, // back 2
    6, 2, 4, // left 1
    0, 4, 2, // left 2
    3, 7, 1, // right 1
    5, 1, 7, // right 2
  };
  // clang-format on

  _basic_entities.push_back({name, vertecies, indecies});
  _basic_entity_dict[name] = _basic_entity_index;
  return _basic_entity_index++;
}

} // namespace Game
