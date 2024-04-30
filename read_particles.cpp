#include <fstream>
#include <iostream>
#include <vector>

struct Particle {
    double position_x, position_y, position_z;
    double hv_vector_x, hv_vector_y, hv_vector_z;
    double speed_x, speed_y, speed_z;
};

template <typename T>
  requires(std::is_integral_v<T> or std::is_floating_point_v<T>)
char * as_writable_buffer(T & value) {
  return reinterpret_cast<char *>(&value);
}

template <typename T>
  requires(std::is_integral_v<T> or std::is_floating_point_v<T>)
T read_binary_value(std::istream & is) {
  T value{};
  is.read(as_writable_buffer(value), sizeof(value));
  return value;
}

struct Particle getParticle(std::ifstream & infile) {
  struct Particle particle;
  particle.position_x  = static_cast<double>(read_binary_value<float>(infile));
  particle.position_y  = static_cast<double>(read_binary_value<float>(infile));
  particle.position_z  = static_cast<double>(read_binary_value<float>(infile));
  particle.hv_vector_x = static_cast<double>(read_binary_value<float>(infile));
  particle.hv_vector_y = static_cast<double>(read_binary_value<float>(infile));
  particle.hv_vector_z = static_cast<double>(read_binary_value<float>(infile));
  particle.speed_x     = static_cast<double>(read_binary_value<float>(infile));
  particle.speed_y     = static_cast<double>(read_binary_value<float>(infile));
  particle.speed_z     = static_cast<double>(read_binary_value<float>(infile));
  return particle;
}

int main(int argc, char * argv[]) {
  if (argc != 2) {
    std::cerr << "Uso: " << argv[0] << " <ruta_del_archivo>" << std::endl;
    return 1;
  }

  char const * filename = argv[1];
  std::ifstream file(filename, std::ios::binary);

  if (!file.is_open()) {
    std::cerr << "No se pudo abrir el archivo: " << filename << std::endl;
    return 1;
  }

  // Leer los bloques
  double particles_per_meter = static_cast<double>(read_binary_value<float>(file));
  int particles_number       = read_binary_value<int>(file);

  std::vector<struct Particle> particles;
  for (int i = 0; i < particles_number; i++) {
    struct Particle p = getParticle(file);
    particles.push_back(p);
  }

  // Imprimir los bloques
  std::cout << "particles_per_meter: " << particles_per_meter << std::endl;
  std::cout << "particles_number: " << particles_number << std::endl;

  for (int i = 0; i < particles_number; i++) {
    std::cout << "particle " << i << std::endl;
    std::cout << "position_x: " << particles[i].position_x << std::endl;
    std::cout << "position_y: " << particles[i].position_y << std::endl;
    std::cout << "position_z: " << particles[i].position_z << std::endl;
    std::cout << "hv_vector_x: " << particles[i].hv_vector_x << std::endl;
    std::cout << "hv_vector_y: " << particles[i].hv_vector_y << std::endl;
    std::cout << "hv_vector_z: " << particles[i].hv_vector_z << std::endl;
    std::cout << "speed_x: " << particles[i].speed_x << std::endl;
    std::cout << "speed_y: " << particles[i].speed_y << std::endl;
    std::cout << "speed_z: " << particles[i].speed_z << std::endl << std::endl;
  }

  file.close();

  return 0;
}