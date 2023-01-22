#include "Duck.hpp"
#include "GameObject.hpp"
#include "Player.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "constants.h"

#include <chrono>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <ostream>
#include <thread>

using namespace Game;

inline void fps_logger(float dur);
inline void build_scene(GameObject& floor, GameObject& tree, GameObject& fence);
inline float sleep_fps(std::chrono::steady_clock::time_point& start,
                       std::chrono::steady_clock::time_point& renderEnd);

int main(int argc, char** argv) {
  Scene scene = Scene();
  Renderer renderer = Renderer(scene);
  renderer.init("Zombie Duck Hunt");
  renderer.addDirectionalLight({1, 1, 1}, 0.2);
  // ------ classic half life blinking
  // renderer.addPointLight({3.5, 1, 2}, "mmamammmmammamamaaamammma");
  // renderer.addPointLight({3.5, 1, 2}, "m");
  // ------ other light effects
  // renderer.addPointLight({3.5, 1, 2}, "jklmnopqrstuvwxyzyxwvutsrqponmlkj");
  renderer.addPointLight({3.5, 1, 2}, "nmonqnmomnmomomno");

  Player player = Player({-1, 0, 0}, 0, 0, renderer.getWindow());
  renderer.bindPlayer(&player);

  std::vector<Duck*> ducks;
  GameObject duck_handler(scene, BUILD_TO_ROOT + DUCK_PATH);
  {
    constexpr u_int32_t duck_amount = 10;
    for (int i = 0; i < duck_amount; i++)
      ducks.push_back(new Duck(&duck_handler, &player));
  }

  GameObject floor(scene, BUILD_TO_ROOT + FLOOR_PATH);
  GameObject fence(scene, BUILD_TO_ROOT + FENCE_PATH);
  GameObject tree(scene, BUILD_TO_ROOT + TREE_PATH);
  build_scene(floor, tree, fence);

  GameObject lamp(scene, BUILD_TO_ROOT + LAMP_PATH);
  lamp.spawn({4, -2, 0}, {1, 0, 0, 0});

  GameObject katana(scene, BUILD_TO_ROOT + KATANA_PATH);
  player.addKatana(katana);

  float frame_time, lastFrame = 0.f;
  while (renderer.shouldRun()) {
    auto start = std::chrono::high_resolution_clock::now();
    fps_logger(lastFrame);
    if (player.getMousePressed())
      lastFrame *= SLOW_MO_MULT;

    {
      int duck_removed = -1; // support to one deletion per frame
      for (int i = 0; i < ducks.size(); i++) {
        ducks[i]->update(lastFrame, ducks);
        if (ducks[i]->shouldRemove())
          duck_removed = i;
      }
      if (duck_removed >= 0) {
        delete ducks[duck_removed];
        ducks[duck_removed] = new Duck(&duck_handler, &player);
        // ducks.erase(ducks.begin() + duck_removed);
      }
    }

    renderer.renderFrame(lastFrame);

    if (player.getHealth() < 0.f) {
      std::cout << "\n----- YOU LOSE! -----" << std::endl;
      return 0;
    }

    auto renderEnd = std::chrono::high_resolution_clock::now();
    lastFrame = sleep_fps(start, renderEnd);
  }
  return 0;
}

inline float sleep_fps(std::chrono::steady_clock::time_point& start,
                       std::chrono::steady_clock::time_point& renderEnd) {
  float renderTime =
      std::chrono::duration_cast<std::chrono::milliseconds>(renderEnd - start)
          .count();
  if (renderTime < FRAME_TIME_MS)
    std::this_thread::sleep_for(
        std::chrono::milliseconds((int)(FRAME_TIME_MS - renderTime)));

  auto end = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
      .count();
}

inline void build_scene(GameObject& floor, GameObject& tree,
                        GameObject& fence) {

  floor.spawn({0, 0, 0}, {1, 0, 0, 0});

  float barrier = SCENE_SIZE - SCENE_OFFSET;
  for (float i = -barrier; i < barrier; i += 4.f) {
    fence.spawn({i + 3, 0.f, barrier}, {1, 0, 0, 0});
    fence.spawn({i + 3, 0.f, -barrier}, {1, 0, 0, 0});
    tree.spawn({i, 0.f, barrier + 4.f}, {1, 0, 0, 0});
    tree.spawn({i, 0.f, -barrier - 4.f}, {1, 0, 0, 0});
  }

  glm::quat rot = glm::rotate(1.57f, glm::vec3({0, 1, 0}));

  for (float i = -barrier; i < barrier; i += 4.f) {
    fence.spawn({barrier, 0.f, i}, rot);
    fence.spawn({-barrier, 0.f, i}, rot);
    tree.spawn({barrier + 4.f, 0.f, i}, {1, 0, 0, 0});
    tree.spawn({-barrier - 4.f, 0.f, i}, {1, 0, 0, 0});
  }
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
