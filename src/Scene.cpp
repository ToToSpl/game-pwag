#include "Scene.hpp"
#include "Texture.hpp"
#include "constants.h"
#include "glm/gtx/transform.hpp"
#include <OpenGL/OpenGL.h>
#include <cmath>

#include <iostream>

namespace Game {
Scene::Scene() {}
Scene::~Scene() {} // if only one scene is used, we don't need to clear memory

u_int32_t Scene::addBoxEntity(json& data, std::string configPath) {
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
    // TODO: propably not the most efficient placement
    // figure how to decrease vertex nomber, and texture works
    position[0] - dimension[0], position[1] + dimension[1], position[2] + dimension[2], // front LU
    position[0] + dimension[0], position[1] + dimension[1], position[2] + dimension[2], // front RU
    position[0] - dimension[0], position[1] - dimension[1], position[2] + dimension[2], // front LD
    position[0] + dimension[0], position[1] - dimension[1], position[2] + dimension[2], // front RD

    position[0] - dimension[0], position[1] + dimension[1], position[2] - dimension[2], // back LU
    position[0] + dimension[0], position[1] + dimension[1], position[2] - dimension[2], // back RU
    position[0] - dimension[0], position[1] - dimension[1], position[2] - dimension[2], // back LD
    position[0] + dimension[0], position[1] - dimension[1], position[2] - dimension[2], // back RD   

    position[0] - dimension[0], position[1] + dimension[1], position[2] - dimension[2], // left LU
    position[0] - dimension[0], position[1] + dimension[1], position[2] + dimension[2], // left RU
    position[0] - dimension[0], position[1] - dimension[1], position[2] - dimension[2], // left LD
    position[0] - dimension[0], position[1] - dimension[1], position[2] + dimension[2], // left RD

    position[0] + dimension[0], position[1] + dimension[1], position[2] - dimension[2], // right LU
    position[0] + dimension[0], position[1] + dimension[1], position[2] + dimension[2], // right RU
    position[0] + dimension[0], position[1] - dimension[1], position[2] - dimension[2], // right LD
    position[0] + dimension[0], position[1] - dimension[1], position[2] + dimension[2], // right RD

    position[0] - dimension[0], position[1] - dimension[1], position[2] + dimension[2], // bottom LU
    position[0] + dimension[0], position[1] - dimension[1], position[2] + dimension[2], // bottom RU
    position[0] - dimension[0], position[1] - dimension[1], position[2] - dimension[2], // bottom LD
    position[0] + dimension[0], position[1] - dimension[1], position[2] - dimension[2], // bottom RD

    position[0] - dimension[0], position[1] + dimension[1], position[2] + dimension[2], // top LU
    position[0] + dimension[0], position[1] + dimension[1], position[2] + dimension[2], // top RU
    position[0] - dimension[0], position[1] + dimension[1], position[2] - dimension[2], // top LD
    position[0] + dimension[0], position[1] + dimension[1], position[2] - dimension[2], // top RD
    /*
    position[0] - dimension[0], position[1] - dimension[1], position[2] + dimension[2], // left bottom front
    position[0] + dimension[0], position[1] - dimension[1], position[2] + dimension[2], // right bottom front
    position[0] + dimension[0], position[1] + dimension[1], position[2] + dimension[2], // right top front
    position[0] - dimension[0], position[1] + dimension[1], position[2] + dimension[2], // left top front
    position[0] - dimension[0], position[1] - dimension[1], position[2] - dimension[2], // left bottom back
    position[0] + dimension[0], position[1] - dimension[1], position[2] - dimension[2], // right bottom back
    position[0] + dimension[0], position[1] + dimension[1], position[2] - dimension[2], // right top back
    position[0] - dimension[0], position[1] + dimension[1], position[2] - dimension[2], // left top back
    */
  };

  indecies = (u_int16_t[CUBE_IND]){
    // front
    2, 1, 0,
    2, 3, 1,
    // back
    4, 5, 6,
    5, 7, 6,
    // left
    10, 9, 8,
    10, 11, 9,
    // right
    12, 13, 14,
    13, 15, 14,
    // bottom
    18, 17, 16,
    18, 19, 17,
    // top
    20, 21, 22,
    21, 23, 22,
    /*
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
		*/
  };
  // clang-format on

  BasicEntity ent;
  ent.name = name;
  ent.vertecies = vertecies;
  ent.indecies = indecies;
  ent.indeciesSize = CUBE_IND;

  for (uint8_t i = 0; i < 3; i++) {
    ent.dimension[i] = dimension[i];
    ent.position[i] = position[i];
  }

  if (data["special"] == "none")
    ent.special = EntitySpecial::NONE;
  else if (data["special"] == "flap")
    ent.special = EntitySpecial::FLAP;

  glGenBuffers(1, &ent.vertArr);
  glBindBuffer(GL_ARRAY_BUFFER, ent.vertArr);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * CUBE_VERT, vertecies,
               GL_STATIC_DRAW);

  glGenBuffers(1, &ent.indArr);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ent.indArr);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u_int16_t) * CUBE_IND, indecies,
               GL_STATIC_DRAW);

  // texture
  textureCreateBox(&ent.texture, data["texture"], configPath);

  _basic_entities.push_back(ent);
  _basic_entity_dict[name] = _basic_entity_index;
  return _basic_entity_index++;
}

SceneObject* Scene::spawnEntity(u_int32_t entId, glm::vec3 pos, glm::quat rot) {
  SceneObject* obj = new SceneObject;

  glm::mat4 rotM = glm::toMat4(rot);
  glm::mat4 trans = glm::translate(glm::mat4(1.0f), pos);
  obj->transform = trans * rotM;
  _basic_entities[entId].objects.push_back(obj);

  return obj;
}

void Scene::renderEntityObjects(BasicEntity& ent, GLuint camMatID,
                                glm::mat4& camMat, glm::vec3& camPos,
                                GLuint transMatID, GLuint normalMatID,
                                GLuint cameraPosID) {

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, ent.vertArr);
  glVertexAttribPointer(0,        // attribute
                        3,        // size
                        GL_FLOAT, // type
                        GL_FALSE, // normalized?
                        0,        // stride
                        (void*)0  // array buffer offset
  );

  textureBindAttrib(&ent.texture, 1);
  textureBindNormals(&ent.texture, 2);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ent.indArr);

  glUniform3fv(cameraPosID, 1, &camPos[0]);

  if (ent.special == EntitySpecial::NONE) {
    for (u_int32_t i = 0; i < ent.objects.size(); i++) {
      // set uniforms
      SceneObject* obj = ent.objects[i];
      glm::mat4 mvp = camMat * obj->transform;
      glm::mat3 normalMat = glm::transpose(glm::inverse(obj->transform));
      glUniformMatrix4fv(camMatID, 1, GL_FALSE, &mvp[0][0]);
      glUniformMatrix4fv(transMatID, 1, GL_FALSE, &obj->transform[0][0]);
      glUniformMatrix3fv(normalMatID, 1, GL_FALSE, &normalMat[0][0]);
      // draw
      glDrawElements(GL_TRIANGLES,      // mode
                     ent.indeciesSize,  // count
                     GL_UNSIGNED_SHORT, // type
                     (void*)0           // element array buffer offset
      );
    }
  } else if (ent.special == EntitySpecial::FLAP) {
    ent.specialVar += 0.1;
    if (ent.specialVar > 6.28)
      ent.specialVar = 0.f;

    for (u_int32_t i = 0; i < ent.objects.size(); i++) {
      float rot = 0.785f * (1.f + sinf(ent.specialVar + (float)i));
      SceneObject* obj = ent.objects[i];
      glm::mat4 flappy = glm::translate(
          obj->transform, {ent.position[0], ent.position[1] + ent.dimension[1],
                           ent.position[2]});
      if (ent.position[0] > 0)
        flappy = glm::rotate(flappy, rot, {0.f, 0.f, 1.f});
      else
        flappy = glm::rotate(flappy, -rot, {0.f, 0.f, 1.f});
      flappy = glm::translate(flappy, {-ent.position[0],
                                       -ent.position[1] - ent.dimension[1],
                                       -ent.position[2]});

      glm::mat4 mvp = camMat * flappy;
      glm::mat3 normalMat = glm::transpose(glm::inverse(flappy));
      glUniformMatrix4fv(camMatID, 1, GL_FALSE, &mvp[0][0]);
      glUniformMatrix4fv(transMatID, 1, GL_FALSE, &obj->transform[0][0]);
      glUniformMatrix3fv(normalMatID, 1, GL_FALSE, &normalMat[0][0]);
      // draw
      glDrawElements(GL_TRIANGLES,      // mode
                     ent.indeciesSize,  // count
                     GL_UNSIGNED_SHORT, // type
                     (void*)0           // element array buffer offset
      );
    }
  }

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}

} // namespace Game
