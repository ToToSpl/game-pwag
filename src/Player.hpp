#pragma once
#include <glm/glm.hpp>

namespace Game {
class Camera {
private:
  float _screenW, _screenH, _fov;
  glm::mat4 _projectionMat;

public:
  Camera();
  ~Camera();
};

class Player {
private:
public:
  Player();
  ~Player();
};
} // namespace Game
