#include "Texture.hpp"
#include "../lib/lodepng.h"
#include "constants.h"
#include <cstring>
#include <iostream>
#include <string>

namespace Game {
void textureCreateBox(Texture* tex, json& data, std::string& configPath) {
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

  {
    // load png
    std::vector<u_int8_t> pngArr;
    if (unsigned error =
            lodepng::decode(pngArr, tex->width, tex->height, texFileName)) {
      std::cout << "PNG decoder error " << error << ": "
                << lodepng_error_text(error) << std::endl;
      abort();
    }
    // based on 4bit png convert to 3bit array
    tex->pixels =
        (u_int8_t*)malloc(sizeof(u_int8_t) * 3 * tex->width * tex->height);
    u_int32_t pngI = 0, arrI = 0;
    while (pngI < pngArr.size()) {
      tex->pixels[arrI++] = pngArr[pngI++];
      tex->pixels[arrI++] = pngArr[pngI++];
      tex->pixels[arrI++] = pngArr[pngI++];
      pngI++;
    }
  }

  // copy json data
  float top[4], bottom[4], left[4], right[4], front[4], back[4];
  {
    float sW = 1.0f / (float)tex->width;
    float sH = 1.0f / (float)tex->height;
    for (u_int8_t i = 0; i < 4; i += 2) {
      top[i] = (float)data["top"][i] * sW;
      top[i + 1] = 1.0f - (float)data["top"][i + 1] * sH;

      bottom[i] = (float)data["bottom"][i] * sW;
      bottom[i + 1] = 1.0f - (float)data["bottom"][i + 1] * sH;

      left[i] = (float)data["left"][i] * sW;
      left[i + 1] = 1.0f - (float)data["left"][i + 1] * sH;

      right[i] = (float)data["right"][i] * sW;
      right[i + 1] = 1.0f - (float)data["right"][i + 1] * sH;

      front[i] = (float)data["front"][i] * sW;
      front[i + 1] = 1.0f - (float)data["front"][i + 1] * sH;

      back[i] = (float)data["back"][i] * sW;
      back[i + 1] = 1.0f - (float)data["back"][i + 1] * sH;
    }
  }

  // setup UVs
  tex->UVs = (float*)malloc(sizeof(float) * CUBE_UV);
  // clang-format off
  // TODO: make this right
  tex->UVs = (float[CUBE_UV]) {
    top[0], top[2], 
    top[0], top[1],
    top[0], top[2], 
    top[0], top[1],
    top[1], top[3], 
    top[0], top[3],
    top[1], top[3], 
    top[0], top[3],
  };
  // clang-format on

  // bind buffer
  glGenBuffers(1, &tex->uvID);
  glBindBuffer(GL_ARRAY_BUFFER, tex->uvID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * CUBE_UV, tex->UVs,
               GL_STATIC_DRAW);

  glGenTextures(1, &tex->textureID);
  glBindTexture(GL_TEXTURE_2D, tex->textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)tex->width,
               (GLsizei)tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void textureBindAttrib(Texture* tex, u_int32_t attrib) {
  glBindTexture(GL_TEXTURE_2D, tex->textureID);
  glEnableVertexAttribArray(attrib);
  glBindBuffer(GL_ARRAY_BUFFER, tex->uvID);
  glVertexAttribPointer(attrib,   // attribute.
                        2,        // size : U+V => 2
                        GL_FLOAT, // type
                        GL_FALSE, // normalized?
                        0,        // stride
                        (void*)0  // array buffer offset
  );
}
} // namespace Game
