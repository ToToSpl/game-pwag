#include "Duck.hpp"
#include "GameObject.hpp"

namespace Game {
Duck::Duck(GameObject handler) : _handler(handler) {
  _entity = handler.spawn(glm::vec3({0, 0, 0}), {1, 0, 0, 0});
}

} // namespace Game
