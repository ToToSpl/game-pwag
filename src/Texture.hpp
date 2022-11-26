#pragma once
#include "../lib/json.hpp"
#include "GL/glew.h"

namespace Game {

using json = nlohmann::json;

struct Texture {
  float *UVs, *normals;
  u_int32_t UVsize;
  u_int8_t* pixels;
  u_int32_t width, height;
  GLuint textureID, uvID, normalID;
};

void textureCreateBox(Texture* tex, json& data, std::string& configPath);
void textureBindAttrib(Texture* tex, u_int32_t attrib);
void textureBindNormals(Texture* tex, u_int32_t attrib);

} // namespace Game
