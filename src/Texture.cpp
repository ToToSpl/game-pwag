#include "Texture.hpp"
#include <iostream>
#include <string>

namespace Game {
void textureCreateBox(Texture* tex, json& data, std::string& configPath) {
  // load png
  // find texture path
  std::string texFileName;
  {
    u_int32_t nameBegin = 0;
    const char* str = configPath.c_str();
    for (u_int32_t i = 0; i < configPath.length(); i++)
      if (str[i] == '/')
        nameBegin = i;
    if (nameBegin > 0 && nameBegin < configPath.length() - 1)
      nameBegin++;
    texFileName = configPath.substr(0, nameBegin) + (std::string)data["source"];
  }
}
} // namespace Game
