#pragma once
#include <string>

#define BUILD_TO_ROOT std::string("../")
#define DUCK_PATH std::string("data/assets/duck/duck.json")
#define FLOOR_PATH std::string("data/assets/floor/floor.json")

#define CUBE_VERT 6 * 4 * 3
#define CUBE_IND 6 * 4 * 3
#define CUBE_UV 6 * 4 * 2
#define CUBE_NORMAL 6 * 4 * 3

#define FOV 45.0f

#define GRAVITY 0.0005f
#define PLAYER_SPEED 0.018f
#define PLAYER_ACC 0.00001f
#define PLAYER_DRAG 0.98989397941f
#define PLAYER_JUMP 0.01f
#define MOUSE_SPEED 0.001f
