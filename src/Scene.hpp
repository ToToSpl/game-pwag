#pragma once

#include <GL/glew.h>
#include <map>
#include <string>
#include <vector>

#include "../lib/json.hpp"

namespace Game {

using json = nlohmann::json;

struct BasicEntity {
  std::string name;
  float* vertecies;
  u_int8_t* indecies;
  GLuint vertArr, indArr;
}; // this element holds vertex and index data about some shape.
   // It can be rerendered many times.

struct SceneObject {};

class Scene {
private:
  std::map<std::string, u_int32_t> _basic_entity_dict;
  u_int32_t _basic_entity_index = 0;
  std::vector<BasicEntity> _basic_entities;

public:
  Scene();
  ~Scene();

public:
  u_int32_t addBoxEntity(json data);
};
} // namespace Game
