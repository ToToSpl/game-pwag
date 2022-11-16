#pragma once

#include <map>
#include <string>
#include <vector>

namespace Game {

struct BasicEntity {
  std::string name;
  float* vertexes;
  u_int32_t* indecies;
}; // this element holds vertex and index data about some shape.
   // It can be rerender many times.

class Scene {
private:
  std::map<std::string, u_int32_t> _basic_entity_dict;
  std::vector<BasicEntity> _basic_entities;

public:
  Scene();
  ~Scene();

public:
};
} // namespace Game
