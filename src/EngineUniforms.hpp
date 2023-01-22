#pragma once
#include <GL/glew.h>

namespace Game {
typedef struct UniformsIDs {
  GLuint cameraID, transformationID, normalMatID, cameraPosID, aliveID;
  GLuint healthID, tsID;
  GLuint lightDirID, lightPointID, lightPointMultID;
} UniformsIDs;

} // namespace Game
