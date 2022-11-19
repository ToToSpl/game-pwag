#include "Scene.hpp"
#include "glm/gtx/transform.hpp"

#define CUBE_VERT 8 * 3
#define CUBE_IND 3 * 2 * 6

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
  float* vertecies = (float*)malloc(sizeof(float) * CUBE_VERT);
  u_int16_t* indecies = (u_int16_t*)malloc(sizeof(u_int16_t) * CUBE_IND);

  // clang-format off
  vertecies = (float[CUBE_VERT]){
    // x y z
    position[0] - dimension[0], position[1] - dimension[1], position[2] + dimension[2], // left bottom front
    position[0] + dimension[0], position[1] - dimension[1], position[2] + dimension[2], // right bottom front
    position[0] + dimension[0], position[1] + dimension[1], position[2] + dimension[2], // right top front
    position[0] - dimension[0], position[1] + dimension[1], position[2] + dimension[2], // left top front
    position[0] - dimension[0], position[1] - dimension[1], position[2] - dimension[2], // left bottom back
    position[0] + dimension[0], position[1] - dimension[1], position[2] - dimension[2], // right bottom back
    position[0] + dimension[0], position[1] + dimension[1], position[2] - dimension[2], // right top back
    position[0] - dimension[0], position[1] + dimension[1], position[2] - dimension[2], // left top back
  };

  indecies = (u_int16_t[CUBE_IND]){
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
  };

  // vertecies = (float[CUBE_VERT]) {
  //  -1.0f, -1.0f, 0.0f,
  //   1.0f, -1.0f, 0.0f,
  //  -1.0f,  1.0f, 0.0f,
  //   1.0f,  1.0f, 0.0f,
  // };
  //
  // indecies = (u_int16_t[CUBE_IND]) {
  //   0, 1, 3,
  //   0, 3, 2
  // };

  // clang-format on

  BasicEntity ent;
  ent.name = name;
  ent.vertecies = vertecies;
  ent.indecies = indecies;

  glGenBuffers(1, &ent.vertArr);
  glBindBuffer(GL_ARRAY_BUFFER, ent.vertArr);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * CUBE_VERT, vertecies,
               GL_STATIC_DRAW);

  glGenBuffers(1, &ent.indArr);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ent.indArr);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u_int16_t) * CUBE_IND, indecies,
               GL_STATIC_DRAW);

  _basic_entities.push_back(ent);
  _basic_entity_dict[name] = _basic_entity_index;
  return _basic_entity_index++;
}

SceneObject* Scene::spawnEntity(u_int32_t entId, glm::vec3 pos, glm::quat rot) {
  SceneObject* obj = new SceneObject;

  glm::mat4 rotM = glm::toMat4(rot);
  glm::mat4 trans = glm::translate(glm::mat4(1.0f), pos);
  obj->transform = rotM * trans;
  _basic_entities[entId].objects.push_back(obj);

  return obj;
}

void Scene::renderEntityObjects(BasicEntity& ent, GLuint matID,
                                glm::mat4& mat) {

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, ent.vertArr);
  glVertexAttribPointer(0,        // attribute
                        3,        // size
                        GL_FLOAT, // type
                        GL_FALSE, // normalized?
                        0,        // stride
                        (void*)0  // array buffer offset
  );
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ent.indArr);

  for (auto obj : ent.objects) {
    // set uniforms
    glm::mat4 mvp = mat * obj->transform;
    glUniformMatrix4fv(matID, 1, GL_FALSE, &mvp[0][0]);
    // draw
    glDrawElements(GL_TRIANGLES,         // mode
                   sizeof(ent.indecies), // count
                   GL_UNSIGNED_SHORT,    // type
                   (void*)0              // element array buffer offset
    );
    obj->transform = glm::rotate(obj->transform, 3.14f * (1.0f / 60.0f),
                                 glm::vec3(0.0, 1.0, 0.0));
  }

  glDisableVertexAttribArray(0);
}

} // namespace Game
