#pragma once
#include <string>

#define BUILD_TO_ROOT std::string("../")
#define DUCK_PATH std::string("data/assets/duck/duck.json")
#define FLOOR_PATH std::string("data/assets/floor/floor.json")
#define KATANA_PATH std::string("data/assets/katana/katana.json")
#define FENCE_PATH std::string("data/assets/fence/fence.json")
#define TREE_PATH std::string("data/assets/tree/tree.json")
#define LAMP_PATH std::string("data/assets/lamp/lamp.json")

#define CUBE_VERT 6 * 4 * 3
#define CUBE_IND 6 * 4 * 3
#define CUBE_UV 6 * 4 * 2
#define CUBE_NORMAL 6 * 4 * 3

#define FOV 45.0f

#define FLAP_SPEED 0.008f

#define GRAVITY 0.0005f
#define PLAYER_SPEED 0.018f
#define PLAYER_ACC 0.00001f
#define PLAYER_DRAG 0.98989397941f
#define PLAYER_JUMP 0.01f
#define PLAYER_ATTACK_RANGE 2.5f
#define PLAYER_ATTACK_ACC 0.7f
#define PLAYER_HEALTH_COOLDOWN 3.f
#define PLAYER_HEALTH_INCREASE 5.f
#define MOUSE_SPEED 0.001f
#define CAMERA_POS_REL                                                         \
  { 0.f, 1.8f, 0.f }

#define KATANA_POS_REL                                                         \
  { -0.2, -0.25, 0.75 }
#define KATANA_BLOOD_TIME 2.f

#define FPS_AVERAGE 10 // running average for fps calculation
#define SLOW_MO_MULT 0.3f
#define FRAME_RATE 45.0f
#define FRAME_TIME_MS (1000.f / FRAME_RATE)

#define DUCK_START_POSITION                                                    \
  { 20, 10, 0 }
#define DUCK_CIRCLE_VEC                                                        \
  { 2, 0, 0 }
#define DUCK_SPEED 2.0f
#define DUCK_AGRO_DISTANCE 15.f
#define DUCK_ATTACK_DISTANCE 1.0f
#define DUCK_ATTACK_STRENGHT 10.f
#define DUCK_COOLDOWN_TIME 1.f
#define DUCK_REPEL_FORCE 0.2f

#define SCENE_SIZE 100.f / 2.f
#define SCENE_OFFSET 10.f

#define LIGHT_BLINK_FPS 10.0f
