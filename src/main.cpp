#include "GameObject.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "constants.h"

#include <iostream>
#include <ostream>
#define FPS_AVERAGE 10

inline void fps_logger(float dur);

using namespace Game;

int main(int argc, char** argv) {
  Renderer renderer = Renderer();
  renderer.init("Zombie Duck Hunt");
  Scene scene = Scene();
  GameObject duck(scene, BUILD_TO_ROOT + DUCK_PATH);

  while (renderer.shouldRun()) {
    fps_logger(renderer.renderFrame());
  }
  return 0;
}

inline void fps_logger(float dur) {
  static float durations[FPS_AVERAGE];
  static u_int32_t duration_step = 0;
  durations[duration_step] = dur;
  if (++duration_step == FPS_AVERAGE) {
    duration_step = 0;
    float time = 0.0f;
    for (int i = 0; i < FPS_AVERAGE; i++)
      time += durations[i];
    time /= FPS_AVERAGE;

    std::cout << "\r" << 1.0f / (0.001 * time) << "\tFPS" << std::flush;
  }
}
