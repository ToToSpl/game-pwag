#pragma once
#include <string>

#define BUILD_TO_ROOT std::string("../")
#define DUCK_PATH std::string("data/assets/duck/duck.json")
#define FLOOR_PATH std::string("data/assets/floor/floor.json")

#define CUBE_VERT 6 * 4 * 3
#define CUBE_IND 6 * 4 * 3
#define CUBE_UV 6 * 4 * 2

#define FOV 45.0f

#define GRAVITY 0.0015f
#define PLAYER_SPEED 0.00007f
#define PLAYER_DRAG 0.4f
#define PLAYER_JUMP 0.03f
#define MOUSE_SPEED 0.001f
