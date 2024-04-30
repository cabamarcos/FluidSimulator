#ifndef GRID_HPP
#define GRID_HPP

#include "block.hpp"
#include "utils.hpp"

#include <string>
#include <vector>

namespace fluids::sim {
  template <typename T>
    requires(std::is_integral_v<T> or std::is_floating_point_v<T>)
  char * as_writable_buffer(T & value);

  template <typename T>
    requires(std::is_integral_v<T> or std::is_floating_point_v<T>)
  char const * as_buffer(T const & value);

  template <typename T>
    requires(std::is_integral_v<T> or std::is_floating_point_v<T>)
  T read_binary_value(std::istream & is);

  template <typename T>
    requires(std::is_integral_v<T> or std::is_floating_point_v<T>)
  void write_binary_value(T value, std::ostream & os);

  // Estrucutra de la particula
  struct Particle {
      Vector3D position     = Vector3D(0, 0, 0);      // Posicion p
      Vector3D hv_vector    = Vector3D(0, 0, 0);      // Coordenadas del vector hv
      Vector3D speed        = Vector3D(0, 0, 0);      // Coordenadas de la velocidad
      double density        = 0;                      // Densidad de la particula iniciacion
      Vector3D acceleration = external_acceleration;  // Coordenadas de la aceleracion
  };

  struct File {
      double particles_per_meter             = 0;
      std::vector<struct Particle> particles = std::vector<struct Particle>();
  };

  struct File readFile(std::string & filename);

  class Grid {
    public:
      struct File file;
      double mass;                // Masa de la particula
      double smoothing_lenght;    // Longitud de suavizado
      double smoothing_6;         // Longitud de suavizado a la 6
      double smoothing_6_pi_15;   // 15 / Longitud de suavizado a la 6 * pi
      double smoothing_6_pi;      // Longitud de suavizado a la 6 * pi
      double smoothing_9;         // Longitud de suavizado a la 9
      double smoothing_2;         // Longitud de suavizado a la 9
      double pressure_rigdity_3;  // 3 * rigidez de la presion
      double viscosity_45;        // 45 * viscosidad
      double fluid_density_2;     // 2 * densidad del fluido
      double time_2;              // Tiempo 2
      double time_squared;        // Tiempo al cuadrado
      double density_transformation_constant;
      double viscosity_mass_h6_pi;
      double mass_pressure;

      int n_blocks_x, n_blocks_y, n_blocks_z;   // Numero de bloques en cada eje
      Vector3D block_size = Vector3D(0, 0, 0);  // Tamaño de cada bloque en cada eje
      std::vector<Block> blocks;                // Bloques

      void generateBlocks();

      Grid(struct File & file);

      [[nodiscard]] Vector3D blockIndex(struct Particle particle) const;
      [[nodiscard]] std::vector<Vector3D> getNeighbours(Vector3D position) const;

      void makeSimulation();

      void evalDensities(size_t block_i);
      void evalAccelerations(size_t block_i);
      void transformDensities(Block & block);
      void evaluateDensities(size_t i, size_t j);
      void evaluateAccelerations(size_t i, size_t j);
      void collisionsX(size_t index, bool x_0);
      void collisionsY(size_t index, bool y_0);
      void collisionsZ(size_t index, bool z_0);
      void collisions(Block & block);
      void updateParticle(Block & block);
      void interactionsX(size_t index, bool x_0);
      void interactionsY(size_t index, bool y_0);
      void interactionsZ(size_t index, bool z_0);
      void interactions(Block & block);

      void writeSimulation(std::string & filename);
  };
}  // namespace fluids::sim
#endif  // GRID_HPP
