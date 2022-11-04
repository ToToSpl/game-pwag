#include "GameObject.hpp"
#include "Scene.hpp"
#include "constants.h"

using namespace Game;

int main(int argc, char** argv) {
  Scene scene = Scene();
  GameObject duck(scene, BUILD_TO_ROOT + DUCK_PATH);

  return 0;
}
