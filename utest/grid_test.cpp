#include "cmath"
#include "sim/block.hpp"
#include "sim/grid.hpp"

#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <string>

using namespace std;
using namespace fluids::sim;

struct Alt_particle {
    int64_t id;
    double posx, posy, posz;
    double hvx, hvy, hvz;
    double velx, vely, velz;
    double density;
    double accx, accy, accz;
};

class GridTest : public ::testing::Test {
  protected:
    // NOLINTNEXTLINE(readability-function-size)
    static Grid grid_from_trz(string & filename) {
      string filename2   = "./inputs/small.fld";
      struct File f_test = readFile(filename2);
      Grid grid(f_test);
      std::ifstream file(filename, std::ios::binary);
      if (!file) { throw std::runtime_error("No se pudo abrir el archivo"); }
      int32_t num_blocks = 0;
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      file.read(reinterpret_cast<char *>(&num_blocks), sizeof(num_blocks));
      std::vector<struct Alt_particle> particles;
      for (Block & block : grid.blocks) {
        block.particles.clear();
        int64_t num_particles = 0;
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        file.read(reinterpret_cast<char *>(&num_particles), sizeof(num_particles));

        for (int64_t i = 0; i < num_particles; i++) {
          struct Alt_particle particle = {};
          // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
          file.read(reinterpret_cast<char *>(&particle.id), sizeof(particle.id));
          // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
          file.read(reinterpret_cast<char *>(&particle.posx), sizeof(particle.posx));
          // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
          file.read(reinterpret_cast<char *>(&particle.posy), sizeof(particle.posy));
          // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
          file.read(reinterpret_cast<char *>(&particle.posz), sizeof(particle.posz));
          // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
          file.read(reinterpret_cast<char *>(&particle.hvx), sizeof(particle.hvx));
          // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
          file.read(reinterpret_cast<char *>(&particle.hvy), sizeof(particle.hvy));
          // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
          file.read(reinterpret_cast<char *>(&particle.hvz), sizeof(particle.hvz));
          // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
          file.read(reinterpret_cast<char *>(&particle.velx), sizeof(particle.velx));
          // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
          file.read(reinterpret_cast<char *>(&particle.vely), sizeof(particle.vely));
          // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
          file.read(reinterpret_cast<char *>(&particle.velz), sizeof(particle.velz));
          // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
          file.read(reinterpret_cast<char *>(&particle.density), sizeof(particle.density));
          // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
          file.read(reinterpret_cast<char *>(&particle.accx), sizeof(particle.accx));
          // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
          file.read(reinterpret_cast<char *>(&particle.accy), sizeof(particle.accy));
          // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
          file.read(reinterpret_cast<char *>(&particle.accz), sizeof(particle.accz));
        }
      }
      std::sort(particles.begin(), particles.end(),
                [](Alt_particle const & a, Alt_particle const & b) {
                  return a.id < b.id;
                });

      std::vector<Particle> particles2;
      for (Alt_particle const & particle : particles) {
        Particle new_particle;
        new_particle.position.x     = particle.posx;
        new_particle.position.y     = particle.posy;
        new_particle.position.z     = particle.posz;
        new_particle.hv_vector.x    = particle.hvx;
        new_particle.hv_vector.y    = particle.hvy;
        new_particle.hv_vector.z    = particle.hvz;
        new_particle.speed.x        = particle.velx;
        new_particle.speed.y        = particle.vely;
        new_particle.speed.z        = particle.velz;
        new_particle.density        = particle.density;
        new_particle.acceleration.x = particle.accx;
        new_particle.acceleration.y = particle.accy;
        new_particle.acceleration.z = particle.accz;
        particles2.push_back(new_particle);
      }
      grid.file.particles = particles2;
      return grid;
    }

    // NOLINTNEXTLINE(readability-function-size,-warnings-as-errors)
    void SetUp() override {
      struct File file;
      string const filename = "input.txt";
      ofstream outputf(filename);
      file.particles_per_meter = 1;
      int particles_number     = 1;
      file.particles.resize(1);
      // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
      file.particles[0].position = Vector3D(1, 2, 3);
      // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
      file.particles[0].hv_vector = Vector3D(4, 5, 6);
      // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers,-warnings-as-errors)
      file.particles[0].speed = Vector3D(7, 8, 9);
      auto value              = static_cast<float>(file.particles_per_meter);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outputf.write(reinterpret_cast<char *>(&value), sizeof(float));
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outputf.write(reinterpret_cast<char *>(&particles_number), sizeof(int));
      value = static_cast<float>(file.particles[0].position.x);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outputf.write(reinterpret_cast<char *>(&value), sizeof(float));
      value = static_cast<float>(file.particles[0].position.y);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outputf.write(reinterpret_cast<char *>(&value), sizeof(float));
      value = static_cast<float>(file.particles[0].position.z);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outputf.write(reinterpret_cast<char *>(&value), sizeof(float));
      value = static_cast<float>(file.particles[0].hv_vector.x);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outputf.write(reinterpret_cast<char *>(&value), sizeof(float));
      value = static_cast<float>(file.particles[0].hv_vector.y);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outputf.write(reinterpret_cast<char *>(&value), sizeof(float));
      value = static_cast<float>(file.particles[0].hv_vector.z);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outputf.write(reinterpret_cast<char *>(&value), sizeof(float));
      value = static_cast<float>(file.particles[0].speed.x);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outputf.write(reinterpret_cast<char *>(&value), sizeof(float));
      value = static_cast<float>(file.particles[0].speed.y);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outputf.write(reinterpret_cast<char *>(&value), sizeof(float));
      value = static_cast<float>(file.particles[0].speed.z);
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
      outputf.write(reinterpret_cast<char *>(&value), sizeof(float));
      outputf.close();
    }

    void TearDown() override {
      if (remove("input.txt") != 0) { perror("Error deleting file"); }
    }
};

TEST_F(GridTest, ReadFileTest) {
  string filename    = "input.txt";
  struct File f_test = readFile(filename);
  ASSERT_EQ(f_test.particles_per_meter, f_test.particles_per_meter);
  ASSERT_EQ(f_test.particles.size(), f_test.particles.size());
  ASSERT_EQ(f_test.particles[0].position, f_test.particles[0].position);
  ASSERT_EQ(f_test.particles[0].hv_vector, f_test.particles[0].hv_vector);
  ASSERT_EQ(f_test.particles[0].speed, f_test.particles[0].speed);
  ASSERT_EQ(f_test.particles[0].density, f_test.particles[0].density);
  ASSERT_EQ(f_test.particles[0].acceleration, f_test.particles[0].acceleration);
}

TEST_F(GridTest, GridConstructorTest) {
  string filename    = "input.txt";
  struct File f_test = readFile(filename);
  Grid const grid(f_test);
  double const mass             = fluid_density / pow(grid.file.particles_per_meter, 3);
  double const smoothing_length = radius_multiplicator / grid.file.particles_per_meter;
  Vector3D const bdist          = bmax - bmin;
  Vector3D block_size     = Vector3D(0, 0, 0);
  int const n_blocks_x          = floor(bdist.x / smoothing_length);
  int const n_blocks_y          = floor(bdist.y / smoothing_length);
  int const n_blocks_z          = floor(bdist.z / smoothing_length);
  block_size.x            = bdist.x / n_blocks_x;
  block_size.y            = bdist.y / n_blocks_y;
  block_size.z            = bdist.z / n_blocks_z;
  ASSERT_EQ(grid.mass, mass);
  ASSERT_EQ(grid.smoothing_lenght, smoothing_length);
  ASSERT_EQ(grid.n_blocks_x, n_blocks_x);
  ASSERT_EQ(grid.n_blocks_y, n_blocks_y);
  ASSERT_EQ(grid.n_blocks_z, n_blocks_z);
  ASSERT_EQ(grid.block_size, block_size);
}

TEST_F(GridTest, BlockIndexTest) {
  string filename    = "input.txt";
  struct File f_test = readFile(filename);
  Grid const grid(f_test);
  Vector3D const bindex = grid.blockIndex(f_test.particles[0]);
  ASSERT_EQ(bindex.x, grid.n_blocks_x - 1);
  ASSERT_EQ(bindex.y, grid.n_blocks_y - 1);
  ASSERT_EQ(bindex.z, grid.n_blocks_z - 1);
}

TEST_F(GridTest, InvalidNp) {
  string filename = "input1.txt";
  ofstream outputf(filename);
  float value          = 1;
  int particles_number = 0;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  outputf.write(reinterpret_cast<char *>(&value), sizeof(value));
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  outputf.write(reinterpret_cast<char *>(&particles_number), sizeof(particles_number));
  outputf.close();

  ASSERT_DEATH(readFile(filename), "Error: Invalid number of particles: 0");
  if (remove("input1.txt") != 0) { perror("Error deleting file"); }
}

TEST_F(GridTest, DifferentNp) {
  string filename = "input2.txt";
  ofstream outputf(filename);
  float value          = 1;
  int particles_number = 2;
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  outputf.write(reinterpret_cast<char *>(&value), sizeof(value));
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  outputf.write(reinterpret_cast<char *>(&particles_number), sizeof(particles_number));
  outputf.close();

  ASSERT_DEATH(readFile(filename), "Error: Number of particles mismatch. Header: 2, Found: 0");
  if (remove("input2.txt") != 0) { perror("Error deleting file"); }
}

TEST_F(GridTest, WriteSimulation) {
  string inputFilename  = "input.txt";
  string outputFilename = "simulation.txt";
  struct File f_test    = readFile(inputFilename);
  Grid grid(f_test);
  grid.writeSimulation(outputFilename);
  ifstream intputF(inputFilename);
  ifstream outputF(outputFilename);
  string line1;
  string line2;
  while (getline(intputF, line1) && getline(outputF, line2)) { ASSERT_EQ(line1, line2); }
  intputF.close();
  outputF.close();

  if (remove("simulation.txt") != 0) { perror("Error deleting file"); }
}

TEST_F(GridTest, EvalDensities) {
  /* Evalúa tanto el incremento, como la transofrmación de densidades */
  string inputFilename  = "./trz/small/repos-base-1.trz";
  string outputFilename = "./trz/small/densinc-base-1.trz";
  Grid small_repos      = grid_from_trz(inputFilename);
  Grid small_densinc    = grid_from_trz(outputFilename);
  for (size_t i = 0; i < small_repos.blocks.size(); i++) { small_repos.evalDensities(i); }
  for (size_t i = 0; i < small_repos.file.particles.size(); i++) {
    ASSERT_EQ(small_repos.file.particles[i].density, small_densinc.file.particles[i].density);
  }
}

TEST_F(GridTest, TransformDensities) {
  /* Evalúa tanto el incremento, como la transofrmación de densidades */
  string inputFilename  = "./trz/small/densinc-base-1.trz";
  string outputFilename = "./trz/small/denstransf-base-1.trz";
  Grid small_densinc    = grid_from_trz(inputFilename);
  Grid small_denstransf = grid_from_trz(outputFilename);
  for (Block & block : small_densinc.blocks) { small_densinc.transformDensities(block); }
  for (size_t i = 0; i < small_densinc.file.particles.size(); i++) {
    ASSERT_EQ(small_densinc.file.particles[i].density, small_denstransf.file.particles[i].density);
  }
}

TEST_F(GridTest, EvalAccelerations) {
  /* Evalúa tanto el incremento, como la transofrmación de densidades */
  string inputFilename  = "./trz/small/denstransf-base-1.trz";
  string outputFilename = "./trz/small/acctransf-base-1.trz";
  Grid small_denstransf = grid_from_trz(inputFilename);
  Grid small_acctransf  = grid_from_trz(outputFilename);
  for (size_t i = 0; i < small_denstransf.blocks.size(); i++) {
    small_denstransf.evalAccelerations(i);
  }
  for (size_t i = 0; i < small_denstransf.file.particles.size(); i++) {
    ASSERT_EQ(small_denstransf.file.particles[i].acceleration,
              small_acctransf.file.particles[i].acceleration);
  }
}

TEST_F(GridTest, CollisionsBlock) {
  string inputFilename  = "./trz/small/acctransf-base-1.trz";
  string outputFilename = "./trz/small/partcol-base-1.trz";
  Grid small_acctransf  = grid_from_trz(inputFilename);
  Grid small_partcol    = grid_from_trz(outputFilename);
  for (size_t i = 0; i < small_acctransf.blocks.size(); i++) {
    small_acctransf.collisions(small_acctransf.blocks[i]);
    for (size_t const particle_index : small_acctransf.blocks[i].particles) {
      ASSERT_EQ(small_acctransf.file.particles[particle_index].acceleration,
                small_partcol.file.particles[particle_index].acceleration);
    }
  }
}

TEST_F(GridTest, UpdateParticle) {
  string inputFilename  = "./trz/small/partcol-base-1.trz";
  string outputFilename = "./trz/small/motion-base-1.trz";
  Grid small_partcol    = grid_from_trz(inputFilename);
  Grid small_motion     = grid_from_trz(outputFilename);

  for (Block & block : small_partcol.blocks) {
    small_partcol.updateParticle(block);
    for (size_t const particle_index : block.particles) {
      ASSERT_EQ(small_partcol.file.particles[particle_index].position,
                small_motion.file.particles[particle_index].position);
      ASSERT_EQ(small_partcol.file.particles[particle_index].hv_vector,
                small_motion.file.particles[particle_index].hv_vector);
      ASSERT_EQ(small_partcol.file.particles[particle_index].speed,
                small_motion.file.particles[particle_index].speed);
    }
  }
}

TEST_F(GridTest, Interacitions) {
  string inputFilename  = "./trz/small/motion-base-1.trz";
  string outputFilename = "./trz/small/boundint-base-1.trz";
  Grid small_motion     = grid_from_trz(inputFilename);
  Grid small_boundint   = grid_from_trz(outputFilename);
  for (Block & block : small_motion.blocks) {
    small_motion.interactions(block);
    for (size_t const particle_index : block.particles) {
      ASSERT_EQ(small_motion.file.particles[particle_index].position,
                small_boundint.file.particles[particle_index].position);
      ASSERT_EQ(small_motion.file.particles[particle_index].hv_vector,
                small_boundint.file.particles[particle_index].hv_vector);
      ASSERT_EQ(small_motion.file.particles[particle_index].speed,
                small_boundint.file.particles[particle_index].speed);
    }
  }
}