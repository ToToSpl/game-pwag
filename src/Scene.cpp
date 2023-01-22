#include "Scene.hpp"
#include "Renderer.hpp"
#include "Texture.hpp"
#include "constants.h"
#include "glm/gtx/transform.hpp"
#include <OpenGL/OpenGL.h>
#include <cmath>

#include <cstring>
#include <format>
#include <fstream>
#include <sstream>
#include <string>

#include <iostream>
#include <unordered_map>

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
  else if (data["special"] == "breathing")
    ent.special = EntitySpecial::BREATHE;

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

void insertThreeFvalues(std::vector<u_int16_t>& t1, std::vector<u_int16_t>& t2,
                        std::vector<u_int16_t>& t3, std::string& values) {
  auto delimiter1 = values.find('/');
  std::string token1 = values.substr(0, delimiter1);
  values.erase(0, delimiter1 + 1);
  auto delimiter2 = values.find('/');
  std::string token2 = values.substr(0, delimiter2);
  std::string token3 = values.substr(delimiter2 + 1, values.length() - 1);
  t1.push_back(std::stof(token1) - 1);
  t2.push_back(std::stof(token2) - 1);
  t3.push_back(std::stof(token3) - 1);
};

u_int32_t Scene::addObjEntity(json& data, std::string configPath) {
  BasicEntity ent;
  // find obj path
  std::string objFileName;
  {
    u_int32_t nameBegin = 0;
    const char* str = configPath.c_str();
    for (u_int32_t i = 0; i < configPath.length(); i++)
      if (str[i] == '/')
        nameBegin = i;
    if (nameBegin > 0 && nameBegin < configPath.length() - 1)
      nameBegin++;
    objFileName = configPath.substr(0, nameBegin) + (std::string)data["source"];
  }

  for (u_int8_t i = 0; i < 3; i++)
    ent.position[i] = (float)data["position"][i];

  float scale = data["scale"];

  // load raw vertex data
  std::vector<float> vertecies, UVs, normals;
  std::vector<u_int16_t> indeciesVertex, indeciesUV, indeciesNormals;
  {
    std::ifstream infile(objFileName);
    std::string line;
    std::string s1, s2, s3;
    float x, y, z;

    std::string param;
    while (std::getline(infile, line)) {
      std::istringstream iss(line);
      if (!(iss >> param)) {
        break;
      }

      if (param == "v") {
        iss >> x >> y >> z;
        vertecies.push_back(scale * x + ent.position[0]);
        vertecies.push_back(scale * y + ent.position[1]);
        vertecies.push_back(scale * z + ent.position[2]);
      } else if (param == "vt") {
        iss >> x >> y;
        UVs.push_back(x);
        UVs.push_back(-y);
      } else if (param == "vn") {
        iss >> x >> y >> z;
        normals.push_back(x);
        normals.push_back(y);
        normals.push_back(z);
      } else if (param == "f") {
        iss >> s1 >> s2 >> s3;
        insertThreeFvalues(indeciesVertex, indeciesUV, indeciesNormals, s1);
        insertThreeFvalues(indeciesVertex, indeciesUV, indeciesNormals, s2);
        insertThreeFvalues(indeciesVertex, indeciesUV, indeciesNormals, s3);
      }
    }
  }
  std::vector<float> ordVert, ordUVs, ordNorms;
  std::vector<u_int16_t> ordIndecies;
  {
    std::unordered_map<u_int64_t, u_int16_t> mapData;

    u_int16_t index = 0;
    for (u_int16_t i = 0; i < indeciesVertex.size(); i++) {
      u_int64_t t1 = indeciesVertex[i];
      u_int64_t t2 = indeciesUV[i];
      u_int64_t t3 = indeciesNormals[i];
      u_int64_t key = t3 << 32 | t2 << 16 | t1;

      auto match = mapData.find(key);
      if (match == mapData.end()) {
        mapData[key] = index;
        ordVert.push_back(vertecies[3 * t1 + 0]);
        ordVert.push_back(vertecies[3 * t1 + 1]);
        ordVert.push_back(vertecies[3 * t1 + 2]);
        ordUVs.push_back(UVs[2 * t2 + 0]);
        ordUVs.push_back(UVs[2 * t2 + 1]);
        ordNorms.push_back(normals[3 * t3 + 0]);
        ordNorms.push_back(normals[3 * t3 + 1]);
        ordNorms.push_back(normals[3 * t3 + 2]);
        ordIndecies.push_back(index);
        index++;
      } else {
        ordIndecies.push_back(match->second);
      }
    }
  }

  ent.name = data["name"];
  ent.vertecies = (float*)malloc(ordVert.size() * sizeof(float));
  memcpy(ent.vertecies, &ordVert[0], ordVert.size() * sizeof(float));
  ent.indecies = (u_int16_t*)malloc(ordIndecies.size() * sizeof(u_int16_t));
  memcpy(ent.indecies, &ordIndecies[0], ordIndecies.size() * sizeof(u_int16_t));
  ent.indeciesSize = ordIndecies.size();

  if (data["special"] == "none")
    ent.special = EntitySpecial::NONE;
  else if (data["special"] == "flap")
    ent.special = EntitySpecial::FLAP;
  else if (data["special"] == "breathing")
    ent.special = EntitySpecial::BREATHE;
  else if (data["special"] == "texblending")
    ent.special = EntitySpecial::TEXBLENDING;

  glGenBuffers(1, &ent.vertArr);
  glBindBuffer(GL_ARRAY_BUFFER, ent.vertArr);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ordVert.size(), ent.vertecies,
               GL_STATIC_DRAW);

  glGenBuffers(1, &ent.indArr);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ent.indArr);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u_int16_t) * ordIndecies.size(),
               ent.indecies, GL_STATIC_DRAW);

  textureCreateObj(&ent.texture, data["texture"], configPath, ordUVs, ordNorms,
                   ordVert.size() / 3,
                   ent.special == EntitySpecial::TEXBLENDING);

  _basic_entities.push_back(ent);
  _basic_entity_dict[ent.name] = _basic_entity_index;
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

void Scene::removeEntity(u_int32_t entId, SceneObject* obj) {
  auto& vec = _basic_entities[entId].objects;
  vec.erase(std::remove(vec.begin(), vec.end(), obj), vec.end());
  delete obj;
}

void Scene::renderEntityObjects(float ts_ms, BasicEntity& ent,
                                glm::mat4& camMat, glm::vec3& camPos,
                                UniformsIDs* uniforms) {

  if (ent.objects.size() == 0)
    return;

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, ent.vertArr);
  glVertexAttribPointer(0,        // attribute
                        3,        // size
                        GL_FLOAT, // type
                        GL_FALSE, // normalized?
                        0,        // stride
                        (void*)0  // array buffer offset
  );

  glUniform1i(uniforms->textureID, 0);
  textureBindAttrib(&ent.texture, 1);
  textureBindNormals(&ent.texture, 2);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ent.indArr);

  glUniform3fv(uniforms->cameraID, 1, &camPos[0]);

  if (ent.special == EntitySpecial::TEXBLENDING) {
    glUniform1i(uniforms->blendingID, 1);
    glUniform1f(uniforms->katanaBloodID, 1.0f);
    textureBindBlend(&ent.texture);
  } else
    glUniform1f(uniforms->katanaBloodID, 0.f);

  if (ent.special == EntitySpecial::NONE ||
      ent.special == EntitySpecial::TEXBLENDING) {
    for (u_int32_t i = 0; i < ent.objects.size(); i++) {
      // set uniforms
      SceneObject* obj = ent.objects[i];
      glm::mat4 mvp = camMat * obj->transform;
      if (obj->moved) {
        obj->normalMat = glm::transpose(glm::inverse(obj->transform));
        obj->moved = false;
      }

      glUniformMatrix4fv(uniforms->cameraID, 1, GL_FALSE, &mvp[0][0]);
      glUniformMatrix4fv(uniforms->transformationID, 1, GL_FALSE,
                         &obj->transform[0][0]);
      glUniformMatrix3fv(uniforms->normalMatID, 1, GL_FALSE,
                         &obj->normalMat[0][0]);
      glUniform1i(uniforms->aliveID, obj->alive);

      // draw
      glDrawElements(GL_TRIANGLES,      // mode
                     ent.indeciesSize,  // count
                     GL_UNSIGNED_SHORT, // type
                     (void*)0           // element array buffer offset
      );
    }
  } else if (ent.special == EntitySpecial::FLAP) {
    ent.specialVar += FLAP_SPEED * ts_ms;
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
      glUniformMatrix4fv(uniforms->cameraID, 1, GL_FALSE, &mvp[0][0]);
      glUniformMatrix4fv(uniforms->transformationID, 1, GL_FALSE,
                         &obj->transform[0][0]);
      glUniformMatrix3fv(uniforms->normalMatID, 1, GL_FALSE, &normalMat[0][0]);
      glUniform1i(uniforms->aliveID, obj->alive);
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
