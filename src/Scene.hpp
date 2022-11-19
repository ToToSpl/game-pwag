#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <map>
#include <string>
#include <vector>

#include "../lib/json.hpp"
#include "Texture.hpp"

namespace Game {

using json = nlohmann::json;

struct SceneObject {
  glm::mat4 transform = glm::mat4(1);
  float scale = 1.0;
};

struct BasicEntity {
  std::string name;
  float* vertecies;
  u_int16_t* indecies;
  u_int16_t indeciesSize;
  GLuint vertArr, indArr;
  std::vector<SceneObject*> objects;
  Texture texture;
}; // this element holds vertex and index data about some shape.
   // It can be rerendered many times.

class Scene {
private:
  std::map<std::string, u_int32_t> _basic_entity_dict;
  u_int32_t _basic_entity_index = 0;
  std::vector<BasicEntity> _basic_entities;

public:
  Scene();
  ~Scene();

public:
  u_int32_t addBoxEntity(json& data, std::string configPath);
  SceneObject* spawnEntity(u_int32_t entId, glm::vec3 pos, glm::quat rot);
  void renderEntityObjects(BasicEntity& ent, GLuint matID, glm::mat4& mat);
  inline std::vector<BasicEntity>* getEntities() { return &_basic_entities; };
};
} // namespace Game
