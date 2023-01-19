#include "Texture.hpp"
#include "../lib/lodepng.h"
#include "constants.h"
#include <cstring>
#include <iostream>
#include <string.h>
#include <string>

namespace Game {

void loadTextureFromFile(std::string& filename, Texture* tex,
                         u_int8_t** target) {
  std::vector<u_int8_t> pngArr;
  if (unsigned error =
          lodepng::decode(pngArr, tex->width, tex->height, filename)) {
    std::cout << "PNG decoder error " << error << ": "
              << lodepng_error_text(error) << std::endl;
    abort();
  }
  // based on 4bit png convert to 3bit array
  *target = (u_int8_t*)malloc(sizeof(u_int8_t) * 3 * tex->width * tex->height);
  u_int32_t pngI = 0, arrI = 0;
  while (pngI < pngArr.size()) {
    (*target)[arrI++] = pngArr[pngI++];
    (*target)[arrI++] = pngArr[pngI++];
    (*target)[arrI++] = pngArr[pngI++];
    pngI++;
  }
}

std::string getTextureFileName(std::string& configPath, json& data,
                               std::string target) {
  std::string texFileName;
  u_int32_t nameBegin = 0;
  const char* str = configPath.c_str();
  for (u_int32_t i = 0; i < configPath.length(); i++)
    if (str[i] == '/')
      nameBegin = i;
  if (nameBegin > 0 && nameBegin < configPath.length() - 1)
    nameBegin++;
  texFileName = configPath.substr(0, nameBegin) + (std::string)data[target];

  return texFileName;
}

void textureCreateBox(Texture* tex, json& data, std::string& configPath) {

  std::string texFileName = getTextureFileName(configPath, data, "source");
  loadTextureFromFile(texFileName, tex, &tex->pixels);

  // copy json data
  float top[4], bottom[4], left[4], right[4], front[4], back[4];
  {
    float sW = 1.0f / (float)tex->width;
    float sH = 1.0f / (float)tex->height;
    for (u_int8_t i = 0; i < 4; i++) {
      top[i] = (float)data["top"][i] * sW;
      bottom[i] = (float)data["bottom"][i] * sW;
      left[i] = (float)data["left"][i] * sW;
      right[i] = (float)data["right"][i] * sW;
      front[i] = (float)data["front"][i] * sW;
      back[i] = (float)data["back"][i] * sW;
    }
  }

  tex->UVs = (float*)malloc(sizeof(float) * CUBE_UV);
  tex->normals = (float*)malloc(sizeof(float) * CUBE_NORMAL);

  // clang-format off
  tex->UVs = (float[CUBE_UV]) {
    front[0], front[1], 
    front[2], front[1],
    front[0], front[3], 
    front[2], front[3],

    back[0], back[1], 
    back[2], back[1],
    back[0], back[3], 
    back[2], back[3],

    left[0], left[1], 
    left[2], left[1],
    left[0], left[3], 
    left[2], left[3],

    right[2], right[1],
    right[0], right[1], 
    right[2], right[3],
    right[0], right[3], 

    bottom[0], bottom[1], 
    bottom[2], bottom[1],
    bottom[0], bottom[3], 
    bottom[2], bottom[3],

    top[0], top[1], 
    top[2], top[1],
    top[0], top[3], 
    top[2], top[3],
  };

  tex->normals = (float[CUBE_NORMAL]) {
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,

    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,

    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,

    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,

    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
  };
  // clang-format on

  // bind buffers
  glGenBuffers(1, &tex->uvID);
  glBindBuffer(GL_ARRAY_BUFFER, tex->uvID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * CUBE_UV, tex->UVs,
               GL_STATIC_DRAW);

  glGenBuffers(1, &tex->normalID);
  glBindBuffer(GL_ARRAY_BUFFER, tex->normalID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * CUBE_NORMAL, tex->normals,
               GL_STATIC_DRAW);

  glGenTextures(1, &tex->textureID);
  glBindTexture(GL_TEXTURE_2D, tex->textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)tex->width,
               (GLsizei)tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void textureCreateObj(Texture* tex, json& data, std::string& configPath,
                      std::vector<float>& UVs, std::vector<float>& normals,
                      u_int16_t vertLen, bool blending) {

  {
    auto texFileName = getTextureFileName(configPath, data, "source");
    loadTextureFromFile(texFileName, tex, &tex->pixels);
  }
  if (blending) {
    auto blendFileName = getTextureFileName(configPath, data, "blending");
    loadTextureFromFile(blendFileName, tex, &tex->blend);
  }

  tex->UVs = (float*)malloc(UVs.size() * sizeof(float));
  memcpy(tex->UVs, &UVs[0], UVs.size() * sizeof(float));
  tex->normals = (float*)malloc(normals.size() * sizeof(float));
  memcpy(tex->normals, &normals[0], normals.size() * sizeof(float));

  // bind buffers
  glGenBuffers(1, &tex->uvID);
  glBindBuffer(GL_ARRAY_BUFFER, tex->uvID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertLen * 2, tex->UVs,
               GL_STATIC_DRAW);

  glGenBuffers(1, &tex->normalID);
  glBindBuffer(GL_ARRAY_BUFFER, tex->normalID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertLen * 3, tex->normals,
               GL_STATIC_DRAW);

  glGenTextures(1, &tex->textureID);
  glBindTexture(GL_TEXTURE_2D, tex->textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)tex->width,
               (GLsizei)tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->pixels);

  if (blending) {
    glGenTextures(1, &tex->blendingID);
    glBindTexture(GL_TEXTURE_2D, tex->blendingID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)tex->width,
                 (GLsizei)tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->blend);
  }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void textureBindAttrib(Texture* tex, u_int32_t attrib) {
  glBindTexture(GL_TEXTURE_2D, tex->textureID);
  glEnableVertexAttribArray(attrib);
  glBindBuffer(GL_ARRAY_BUFFER, tex->uvID);
  glVertexAttribPointer(attrib,   // attribute.
                        2,        // size
                        GL_FLOAT, // type
                        GL_FALSE, // normalized?
                        0,        // stride
                        (void*)0  // array buffer offset
  );
}

void textureBindNormals(Texture* tex, u_int32_t attrib) {
  glEnableVertexAttribArray(attrib);
  glBindBuffer(GL_ARRAY_BUFFER, tex->normalID);
  glVertexAttribPointer(attrib,   // attribute.
                        3,        // size
                        GL_FLOAT, // type
                        GL_FALSE, // normalized?
                        0,        // stride
                        (void*)0  // array buffer offset
  );
}

} // namespace Game
