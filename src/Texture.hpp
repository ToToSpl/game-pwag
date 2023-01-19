#pragma once
#include "../lib/json.hpp"
#include "GL/glew.h"

namespace Game {

using json = nlohmann::json;

struct Texture {
  float *UVs, *normals;
  u_int32_t UVsize;
  u_int8_t *pixels, *blend;
  u_int32_t width, height;
  GLuint textureID, uvID, normalID;
};

void textureCreateBox(Texture* tex, json& data, std::string& configPath);
void textureCreateObj(Texture* tex, json& data, std::string& configPath,
                      std::vector<float>& UVs, std::vector<float>& normals,
                      u_int16_t vertLen, bool blending = false);
void textureBindAttrib(Texture* tex, u_int32_t attrib);
void textureBindNormals(Texture* tex, u_int32_t attrib);

} // namespace Game
