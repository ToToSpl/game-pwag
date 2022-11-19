#include "Texture.hpp"
#include "../lib/lodepng.h"
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
  std::vector<unsigned char> image;
  unsigned width, height;

  if (unsigned error = lodepng::decode(image, width, height, texFileName)) {
    std::cout << "PNG decoder error " << error << ": "
              << lodepng_error_text(error) << std::endl;
    abort();
  }

  std::cout << width << " " << height << std::endl;
}
} // namespace Game
