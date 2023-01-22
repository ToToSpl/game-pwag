#pragma once
#include <GL/glew.h>

namespace Game {
typedef struct UniformsIDs {
  GLuint cameraID, transformationID, normalMatID, cameraPosID, aliveID,
      katanaBloodID;
  GLuint healthID, tsID;
  GLuint lightDirID, lightPointID, lightPointMultID;
  GLint textureID, blendingID;
} UniformsIDs;

} // namespace Game
