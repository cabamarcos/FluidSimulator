#include "grid.hpp"

#include "block.hpp"
#include "utils.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

namespace fluids::sim {
  template <typename T>
    requires(std::is_integral_v<T> or std::is_floating_point_v<T>)
  char * as_writable_buffer(T & value) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<char *>(&value);
  }

  template <typename T>
    requires(std::is_integral_v<T> or std::is_floating_point_v<T>)
  char const * as_buffer(T const & value) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    return reinterpret_cast<char const *>(&value);
  }

  template <typename T>
    requires(std::is_integral_v<T> or std::is_floating_point_v<T>)
  T read_binary_value(std::istream & is) {
    T value{};
    is.read(as_writable_buffer(value), sizeof(value));
    return value;
  }

  template <typename T>
    requires(std::is_integral_v<T> or std::is_floating_point_v<T>)
  void write_binary_value(T value, std::ostream & os) {
    os.write(as_buffer(value), sizeof(value));
  }

  Particle getParticle(std::ifstream & infile) {
    struct Particle particle;
    particle.position.x  = static_cast<double>(read_binary_value<float>(infile));
    particle.position.y  = static_cast<double>(read_binary_value<float>(infile));
    particle.position.z  = static_cast<double>(read_binary_value<float>(infile));
    particle.hv_vector.x = static_cast<double>(read_binary_value<float>(infile));
    particle.hv_vector.y = static_cast<double>(read_binary_value<float>(infile));
    particle.hv_vector.z = static_cast<double>(read_binary_value<float>(infile));
    particle.speed.x     = static_cast<double>(read_binary_value<float>(infile));
    particle.speed.y     = static_cast<double>(read_binary_value<float>(infile));
    particle.speed.z     = static_cast<double>(read_binary_value<float>(infile));
    return particle;
  }

  File readFile(std::string & filename) {
    struct File file;
    std::ifstream infile(filename);
    auto particles_per_meter   = read_binary_value<float>(infile);
    int const number_particles = read_binary_value<int>(infile);
    if (number_particles <= 0) {
      std::cerr << "Error: Invalid number of particles: " << number_particles << '\n';
      exit(ERROR_INVALID_PARTICLE_NUMBER);
    }
    file.particles_per_meter = static_cast<double>(particles_per_meter);
    file.particles.resize(number_particles);
    int count = 0;
    while (infile.peek() != EOF) {
      file.particles[count] = getParticle(infile);  // Para cargar menos en cache
      count++;
    }
    if (count != number_particles) {
      std::cerr << "Error: Number of particles mismatch. Header: " << number_particles
                << ", Found: " << count << '\n';
      exit(ERROR_INVALID_PARTICLE_NUMBER);
    }
    infile.close();
    return file;
  }

  void Grid::generateBlocks() {
    for (int i = 0; i < n_blocks_x * n_blocks_y * n_blocks_z; i++) {
      int const posx = i % n_blocks_x;
      int const posy = (i / n_blocks_x) % n_blocks_y;
      int const posz = (i / (n_blocks_x * n_blocks_y)) % n_blocks_z;
      Block const block(posx, posy, posz);
      blocks.push_back(block);
    }
  }

  Grid::Grid(struct File & file)
    : file(file),                                                         // file
      mass(fluid_density / pow(file.particles_per_meter, 3)),             // particle mass
      smoothing_lenght(radius_multiplicator / file.particles_per_meter),  // smoothing length
      smoothing_6(pow(smoothing_lenght, six)),  // smoothing length to the power of 6
      smoothing_6_pi_15(fifteen / (pow(smoothing_lenght, six) * M_PI)),
      smoothing_6_pi(pow(smoothing_lenght, six) * M_PI),
      smoothing_9(pow(smoothing_lenght, nine)), smoothing_2(pow(smoothing_lenght, 2)),
      pressure_rigdity_3(3 * pressure_rigidity),
      viscosity_45(forty_five * viscosity), fluid_density_2(2 * fluid_density), time_2(time_increase / 2),
      time_squared(pow(time_increase, 2)),
      density_transformation_constant((three_fifteen * mass) / (sixty_four * M_PI * smoothing_9)),
      viscosity_mass_h6_pi((viscosity_45 * mass) / (smoothing_6_pi)),
      mass_pressure((mass * pressure_rigdity_3) / 2) {
    Vector3D const bdist = bmax - bmin;

    n_blocks_x = floor(bdist.x / smoothing_lenght);  // grid size
    n_blocks_y = floor(bdist.y / smoothing_lenght);
    n_blocks_z = floor(bdist.z / smoothing_lenght);

    block_size.x = bdist.x / n_blocks_x;  // block size
    block_size.y = bdist.y / n_blocks_y;
    block_size.z = bdist.z / n_blocks_z;

    generateBlocks();

    std::cout << "Number of particles: " << file.particles.size() << "\n";
    std::cout << "Particles per meter: " << file.particles_per_meter << "\n";
    std::cout << "Smoothing length: " << smoothing_lenght << "\n";
    std::cout << "Particle mass: " << mass << "\n";
    std::cout << "Grid size: " << n_blocks_x << " x " << n_blocks_y << " x " << n_blocks_z << "\n";
    std::cout << "Number of blocks: " << blocks.size() << "\n";
    std::cout << "Block size: " << block_size.x << " x " << block_size.y << " x " << block_size.z
              << "\n";
  }

  [[nodiscard]] Vector3D Grid::blockIndex(struct Particle particle) const {
    Vector3D bindex(0, 0, 0);
    bindex.x = static_cast<double>(floor((particle.position.x - bmin.x) / block_size.x));
    bindex.y = static_cast<double>(floor((particle.position.y - bmin.y) / block_size.y));
    bindex.z = static_cast<double>(floor((particle.position.z - bmin.z) / block_size.z));

    bindex.x = bindex.x < 0 ? 0 : bindex.x;
    bindex.x = bindex.x > n_blocks_x - 1 ? n_blocks_x - 1 : bindex.x;
    bindex.y = bindex.y < 0 ? 0 : bindex.y;
    bindex.y = bindex.y > n_blocks_y - 1 ? n_blocks_y - 1 : bindex.y;
    bindex.z = bindex.z < 0 ? 0 : bindex.z;
    bindex.z = bindex.z > n_blocks_z - 1 ? n_blocks_z - 1 : bindex.z;

    return bindex;
  }

  void Grid::evaluateDensities(size_t i, size_t j) {
    double const distance   = (file.particles[i].position - file.particles[j].position).norm();
    double const distance_2 = distance * distance;
    if (distance_2 < smoothing_2) {
      double const smoothing_distance = smoothing_2 - distance_2;

      double const increment_density   = smoothing_distance * smoothing_distance * smoothing_distance;
      file.particles[i].density += increment_density;
      file.particles[j].density += increment_density;
    }
  }

  void Grid::evaluateAccelerations(size_t i, size_t j) {
    double const distance = (file.particles[i].position - file.particles[j].position).norm();
    if (distance * distance < smoothing_2) {
      double const dist_ij                     = std::max(distance, max_distance);

      Vector3D increment_acceleration_ij  = (file.particles[i].position - file.particles[j].position);
      increment_acceleration_ij *= smoothing_6_pi_15;
      increment_acceleration_ij *= mass_pressure;
      double const h_dist_ij     = smoothing_lenght - dist_ij;
      increment_acceleration_ij *= h_dist_ij * h_dist_ij / dist_ij;
      increment_acceleration_ij *=
          (file.particles[i].density + file.particles[j].density - fluid_density_2);
      increment_acceleration_ij +=
          (file.particles[j].speed - file.particles[i].speed) * viscosity_mass_h6_pi;
      increment_acceleration_ij /= (file.particles[i].density * file.particles[j].density);

      file.particles[i].acceleration += increment_acceleration_ij;
      file.particles[j].acceleration -= increment_acceleration_ij;
    }
  }

  void Grid::collisionsX(size_t index, bool x_0) {
    double const new_x =
        file.particles[index].position.x + file.particles[index].hv_vector.x * time_increase;
    if (x_0) {
      double const increment_x = particle_size + bmin.x - new_x;
      if (increment_x > min_increment) {
        file.particles[index].acceleration.x += rigidity_collisions * increment_x;
        file.particles[index].acceleration.x -= dumping * file.particles[index].speed.x;
      }
    } else {
      double const increment_x = new_x - bmax.x + particle_size;
      if (increment_x > min_increment) {
        file.particles[index].acceleration.x -= rigidity_collisions * increment_x;
        file.particles[index].acceleration.x -= dumping * file.particles[index].speed.x;
      }
    }
  }

  void Grid::collisionsY(size_t index, bool y_0) {
    double const new_y =
        file.particles[index].position.y + file.particles[index].hv_vector.y * time_increase;
    if (y_0) {
      double const increment_y = particle_size + bmin.y - new_y;
      if (increment_y > min_increment) {
        file.particles[index].acceleration.y += rigidity_collisions * increment_y;
        file.particles[index].acceleration.y -= dumping * file.particles[index].speed.y;
      }
    } else {
      double const increment_y = new_y - bmax.y + particle_size;
      if (increment_y > min_increment) {
        file.particles[index].acceleration.y -= rigidity_collisions * increment_y;
        file.particles[index].acceleration.y -= dumping * file.particles[index].speed.y;
      }
    }
  }

  void Grid::collisionsZ(size_t index, bool z_0) {
    double const new_z =
        file.particles[index].position.z + file.particles[index].hv_vector.z * time_increase;
    if (z_0) {
      double const increment_z = particle_size + bmin.z - new_z;
      if (increment_z > min_increment) {
        file.particles[index].acceleration.z += rigidity_collisions * increment_z;
        file.particles[index].acceleration.z -= dumping * file.particles[index].speed.z;
      }
    } else {
      double const increment_z = new_z - bmax.z + particle_size;
      if (increment_z > min_increment) {
        file.particles[index].acceleration.z -= rigidity_collisions * increment_z;
        file.particles[index].acceleration.z -= dumping * file.particles[index].speed.z;
      }
    }
  }

  void Grid::collisions(Block & block) {
    if (block.x == 0 || block.x == n_blocks_x - 1) {
      for (size_t const particle_i : block.particles) { collisionsX(particle_i, block.x == 0); }
    }
    if (block.y == 0 || block.y == n_blocks_y - 1) {
      for (size_t const particle_i : block.particles) { collisionsY(particle_i, block.y == 0); }
    }
    if (block.z == 0 || block.z == n_blocks_z - 1) {
      for (size_t const particle_i : block.particles) { collisionsZ(particle_i, block.z == 0); }
    }
  }

  void Grid::updateParticle(Block & block) {
    for (size_t const index : block.particles) {
      file.particles[index].position += file.particles[index].hv_vector * time_increase +
                                        file.particles[index].acceleration * time_squared;
      file.particles[index].speed =
          file.particles[index].hv_vector + file.particles[index].acceleration * time_2;
      file.particles[index].hv_vector += file.particles[index].acceleration * time_increase;
    }
  }

  void Grid::interactionsX(size_t index, bool x_0) {
    double const d_x =
        x_0 ? file.particles[index].position.x - bmin.x : bmax.x - file.particles[index].position.x;
    if (d_x < 0) {
      file.particles[index].position.x   = x_0 ? bmin.x - d_x : bmax.x + d_x;
      file.particles[index].speed.x     *= -1;
      file.particles[index].hv_vector.x *= -1;
    }
  }

  void Grid::interactionsY(size_t index, bool y_0) {
    double const d_y =
        y_0 ? file.particles[index].position.y - bmin.y : bmax.y - file.particles[index].position.y;
    if (d_y < 0) {
      file.particles[index].position.y   = y_0 ? bmin.y - d_y : bmax.y + d_y;
      file.particles[index].speed.y     *= -1;
      file.particles[index].hv_vector.y *= -1;
    }
  }

  void Grid::interactionsZ(size_t index, bool z_0) {
    double const d_z =
        z_0 ? file.particles[index].position.z - bmin.z : bmax.z - file.particles[index].position.z;
    if (d_z < 0) {
      file.particles[index].position.z   = z_0 ? bmin.z - d_z : bmax.z + d_z;
      file.particles[index].speed.z     *= -1;
      file.particles[index].hv_vector.z *= -1;
    }
  }

  void Grid::interactions(Block & block) {
    if (block.x == 0 || block.x == n_blocks_x - 1) {
      for (size_t const particle_i : block.particles) { interactionsX(particle_i, block.x == 0); }
    }
    if (block.y == 0 || block.y == n_blocks_y - 1) {
      for (size_t const particle_i : block.particles) { interactionsY(particle_i, block.y == 0); }
    }
    if (block.z == 0 || block.z == n_blocks_z - 1) {
      for (size_t const particle_i : block.particles) { interactionsZ(particle_i, block.z == 0); }
    }
  }

  [[nodiscard]] std::vector<Vector3D> Grid::getNeighbours(Vector3D position) const {
    std::vector<Vector3D> neighbours;
    for (int i = -1; i < 2; i++) {
      for (int j = -1; j < 2; j++) {
        for (int k = -1; k < 2; k++) {
          Vector3D const new_position = Vector3D(position.x + i, position.y + j, position.z + k);
          if (new_position.x < 0 || new_position.x > n_blocks_x - 1 || new_position.y < 0 ||
              new_position.y > n_blocks_y - 1 || new_position.z < 0 ||
              new_position.z > n_blocks_z - 1) {
            continue;
          }
          neighbours.push_back(new_position);
        }
      }
    }
    return neighbours;
  }

  void Grid::evalDensities(size_t block_i) {
    Vector3D const position = Vector3D(blocks[block_i].x, blocks[block_i].y, blocks[block_i].z);
    std::vector<Vector3D> const neighbours = getNeighbours(position);
    for (size_t const particle_i : blocks[block_i].particles) {
      for (Vector3D const neighbour : neighbours) {
        auto block_j = static_cast<size_t>(neighbour.x + neighbour.y * n_blocks_x +
                                           neighbour.z * n_blocks_x * n_blocks_y);
        if (block_i == block_j) {
          for (size_t const particle_j : blocks[block_j].particles) {
            if (particle_i < particle_j) { evaluateDensities(particle_i, particle_j); }
          }
        } else if (block_i < block_j) {
          for (size_t const particle_j : blocks[block_j].particles) {
            evaluateDensities(particle_i, particle_j);
          }
        }
      }
    }
  }

  void Grid::transformDensities(Block & block) {
    for (size_t const particle_i : block.particles) {
      file.particles[particle_i].density =
          (file.particles[particle_i].density + smoothing_6) * density_transformation_constant;
    }
  }

  void Grid::evalAccelerations(size_t block_i) {
    Vector3D const position = Vector3D(blocks[block_i].x, blocks[block_i].y, blocks[block_i].z);
    std::vector<Vector3D> const neighbours = getNeighbours(position);
    for (size_t const particle_i : blocks[block_i].particles) {
      for (Vector3D const neighbour : neighbours) {
        auto block_j = static_cast<size_t>(neighbour.x + neighbour.y * n_blocks_x +
                                           neighbour.z * n_blocks_x * n_blocks_y);
        if (block_i == block_j) {
          for (size_t const particle_j : blocks[block_j].particles) {
            if (particle_i < particle_j) { evaluateAccelerations(particle_i, particle_j); }
          }
        } else if (block_i < block_j) {
          for (size_t const particle_j : blocks[block_j].particles) {
            evaluateAccelerations(particle_i, particle_j);
          }
        }
      }
    }
  }

  void Grid::makeSimulation() {
    for (size_t i = 0; i < file.particles.size(); i++) {
      file.particles[i].density      = 0;
      file.particles[i].acceleration = external_acceleration;
      Vector3D const index                 = blockIndex(file.particles[i]);
      auto position =
          static_cast<size_t>(index.x + index.y * n_blocks_x + index.z * n_blocks_x * n_blocks_y);
      blocks[position].particles.push_back(i);
    }

    for (size_t block_i = 0; block_i < blocks.size(); block_i++) {
      evalDensities(block_i);
      transformDensities(blocks[block_i]);
    }

    for (size_t block_i = 0; block_i < blocks.size(); block_i++) {
      evalAccelerations(block_i);
      collisions(blocks[block_i]);
      updateParticle(blocks[block_i]);
      interactions(blocks[block_i]);
      blocks[block_i].particles.clear();
    }
  }

  void Grid::writeSimulation(std::string & filename) {
    std::ofstream outfile(filename);
    write_binary_value(static_cast<float>(file.particles_per_meter), outfile);
    write_binary_value(static_cast<int>(file.particles.size()), outfile);
    for (struct Particle const &particle : file.particles) {
      write_binary_value(static_cast<float>(particle.position.x), outfile);
      write_binary_value(static_cast<float>(particle.position.y), outfile);
      write_binary_value(static_cast<float>(particle.position.z), outfile);
      write_binary_value(static_cast<float>(particle.hv_vector.x), outfile);
      write_binary_value(static_cast<float>(particle.hv_vector.y), outfile);
      write_binary_value(static_cast<float>(particle.hv_vector.z), outfile);
      write_binary_value(static_cast<float>(particle.speed.x), outfile);
      write_binary_value(static_cast<float>(particle.speed.y), outfile);
      write_binary_value(static_cast<float>(particle.speed.z), outfile);
    }
  }
}  // namespace fluids::sim