#include "Duck.hpp"
#include "GameObject.hpp"
#include "Player.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "constants.h"

#include <chrono>
#include <iostream>
#include <ostream>
#include <thread>

inline void fps_logger(float dur);

using namespace Game;

int main(int argc, char** argv) {
  Scene scene = Scene();
  Renderer renderer = Renderer(scene);
  renderer.init("Zombie Duck Hunt");

  Player player = Player({-1, 0, 0}, 0, 0, renderer.getWindow());
  renderer.bindPlayer(&player);

  std::vector<Duck*> ducks;
  GameObject duck_handler(scene, BUILD_TO_ROOT + DUCK_PATH);
  {
    constexpr u_int32_t duck_amount = 20;
    for (int i = 0; i < duck_amount; i++) {
      ducks.push_back(new Duck(&duck_handler, &player));
    }
  }

  // duck.spawn({0, 1, 0}, {1, 0, 0, 0});
  // duck.spawn({0, 2.5, 0}, {0, 0, 0, 1});
  // u_int32_t box_side = 4;
  // std::vector<GameEntity*> ducks;
  // for (u_int32_t i = 0; i < box_side; i++) {
  //   for (u_int32_t j = 0; j < box_side; j++) {
  //     for (u_int32_t k = 0; k < box_side; k++) {
  //       ducks.push_back(duck.spawn({i, j + 1.f, k}, {1, 0, 0, 0}));
  //     }
  //   }
  // }
  // duck.kill(ducks[0]);

  GameObject floor(scene, BUILD_TO_ROOT + FLOOR_PATH);
  floor.spawn({0, 0, 0}, {1, 0, 0, 0});

  GameObject katana(scene, BUILD_TO_ROOT + KATANA_PATH);
  player.addKatana(katana);

  float frame_time, lastFrame = 0.f;
  while (renderer.shouldRun()) {
    auto start = std::chrono::high_resolution_clock::now();
    fps_logger(lastFrame);
    if (player.getMousePressed())
      lastFrame *= SLOW_MO_MULT;

    for (auto& d : ducks)
      d->update(lastFrame);

    renderer.renderFrame(lastFrame);

    auto renderEnd = std::chrono::high_resolution_clock::now();
    float renderTime =
        std::chrono::duration_cast<std::chrono::milliseconds>(renderEnd - start)
            .count();
    if (renderTime < FRAME_TIME_MS)
      std::this_thread::sleep_for(
          std::chrono::milliseconds((int)(FRAME_TIME_MS - renderTime)));

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
