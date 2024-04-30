#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>

namespace fluids::sim {
#define ERROR_INVALID_NUMBER_ARGUMENTS (-1)
#define ERROR_INVALID_TIME_STEPS (-1)
#define ERROR_INVALID_NUMBER_TIME_STEPS (-2)
#define ERROR_CANNOT_OPEN_INPUT_FILE (-3)
#define ERROR_CANNOT_OPEN_OUTPUT_FILE (-4)
#define ERROR_INVALID_PARTICLE_NUMBER (-5)

  class Vector3D {
    public:
      double x, y, z;

      Vector3D(double xCoord, double yCoord, double zCoord) : x(xCoord), y(yCoord), z(zCoord) { }

      Vector3D operator+(Vector3D const & v) const { return {x + v.x, y + v.y, z + v.z}; }

      Vector3D operator-(Vector3D const & v) const { return {x - v.x, y - v.y, z - v.z}; }

      Vector3D operator*(double s) const { return {x * s, y * s, z * s}; }

      Vector3D operator/(double s) const { return {x / s, y / s, z / s}; }

      [[nodiscard]] double dot(Vector3D const & v) const { return x * v.x + y * v.y + z * v.z; }

      [[nodiscard]] double norm() const { return sqrt(x * x + y * y + z * z); }

      [[nodiscard]] Vector3D normalized() const { return *this / norm(); }

      Vector3D & operator+=(Vector3D const & v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
      }

      Vector3D & operator-=(Vector3D const & v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
      }

      Vector3D & operator*=(double s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
      }

      Vector3D & operator/=(double s) {
        x /= s;
        y /= s;
        z /= s;
        return *this;
      }

      bool operator==(Vector3D const & v) const { return x == v.x && y == v.y && z == v.z; }

      bool operator!=(Vector3D const & v) const { return x != v.x || y != v.y || z != v.z; }

      Vector3D operator-() const { return {-x, -y, -z}; }
  };

  double const radius_multiplicator = 1.695;  // Multiplicador de radio
  double const fluid_density        = 1e3;    // Densidad del fluido
  double const pressure_rigidity    = 3.0;    // Presion de rigidez
  double const rigidity_collisions  = 3e4;    // Colisiones de rigidez
  double const dumping              = 128.0;  // Amortiguamiento
  double const viscosity            = 0.4;    // Viscosidad
  double const particle_size        = 2e-4;   // Tamaño de particula
  double const time_increase        = 1e-3;   // Paso de tiempo
  double const six = 6;
  double const fifteen = 15;
  double const nine = 9;
  double const forty_five = 45;
  double const sixty_four = 64;
  double const three_fifteen = 315;
  double const max_distance = 1e-6;

  double const min_increment = 1e-10;

  // NOLINTNEXTLINE(cert-err58-cpp)
  Vector3D const external_acceleration(0.0, -9.8, 0.0);  // Aceleracion externa
  // NOLINTNEXTLINE(cert-err58-cpp)
  Vector3D const bmax(0.065, 0.1, 0.065);  // Limite superior de recinto
  // NOLINTNEXTLINE(cert-err58-cpp)
  Vector3D const bmin(-0.065, -0.08, -0.065);  // Limite inferior de recinto
}  // namespace fluids::sim
#endif
