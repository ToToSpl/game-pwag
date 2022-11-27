#include "GameObject.hpp"
#include "Player.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "constants.h"

#include <chrono>
#include <iostream>
#include <ostream>
#include <thread>
#define FPS_AVERAGE 10 // running average for fps calculation

inline void fps_logger(float dur);

using namespace Game;

int main(int argc, char** argv) {
  Scene scene = Scene();
  Renderer renderer = Renderer(scene);
  renderer.init("Zombie Duck Hunt");

  Player player = Player({-1, 0, 0}, 0, 0, renderer.getWindow());
  renderer.bindPlayer(&player);

  GameObject duck(scene, BUILD_TO_ROOT + DUCK_PATH);
  // duck.spawn({0, 1, 0}, {1, 0, 0, 0});
  // duck.spawn({0, 2.5, 0}, {0, 0, 0, 1});
  u_int32_t box_side = 4;
  for (u_int32_t i = 0; i < box_side; i++) {
    for (u_int32_t j = 0; j < box_side; j++) {
      for (u_int32_t k = 0; k < box_side; k++) {
        duck.spawn({i, j + 1.f, k}, {1, 0, 0, 0});
      }
    }
  }

  GameObject floor(scene, BUILD_TO_ROOT + FLOOR_PATH);
  floor.spawn({0, 0, 0}, {1, 0, 0, 0});

  GameObject katana(scene, BUILD_TO_ROOT + KATANA_PATH);
  katana.spawn({0, 0.9, -1}, {0, 1, 0, 0});

  float frame_time, lastFrame = 0.f;
  while (renderer.shouldRun()) {
    auto start = std::chrono::high_resolution_clock::now();
    fps_logger(lastFrame);

    renderer.renderFrame(lastFrame);

    auto end = std::chrono::high_resolution_clock::now();
    lastFrame =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();
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

    std::cout << "\r" << 1.f / (0.001f * time) << "\tFPS" << std::flush;
  }
}
