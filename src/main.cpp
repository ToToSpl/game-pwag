#include "GameObject.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "constants.h"

#include <iostream>
#include <ostream>
#define FPS_AVERAGE 10 // running average for fps calculation

inline void fps_logger(float dur);

using namespace Game;

int main(int argc, char** argv) {
  Scene scene = Scene();
  Renderer renderer = Renderer(scene);
  renderer.init("Zombie Duck Hunt");
  GameObject duck(scene, BUILD_TO_ROOT + DUCK_PATH);
  duck.spawn({0, 0, 0}, {1, 0, 0, 0});
  duck.spawn({0, 1.5, 0}, {0, 0, 0, 1});

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
