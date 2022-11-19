#pragma once
#include "../lib/json.hpp"
#include "GL/glew.h"

namespace Game {

using json = nlohmann::json;

struct Texture {
  float* UVs;
  u_int32_t UVsize;
  GLuint textureID;
};

void textureCreateBox(Texture* tex, json& data, std::string& configPath);

} // namespace Game
