#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <vector>

namespace fluids::sim {
  class Block {
    public:
      int x, y, z;
      std::vector<size_t> particles = std::vector<size_t>();
      Block(int x_coord, int y_coord, int z_coord) : x(x_coord), y(y_coord), z(z_coord){};
  };
}  // namespace fluids::sim

#endif  // BLOCK_HPP