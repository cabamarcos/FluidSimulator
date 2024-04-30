#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

struct Particle {
    int64_t id;
    double posx, posy, posz;
    double hvx, hvy, hvz;
    double velx, vely, velz;
    double density;
    double acc[3];
};

struct Block {
    int64_t num_particles;
    std::vector<Particle> particles;
};

std::vector<Block> read_file(std::string const & filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file) { throw std::runtime_error("No se pudo abrir el archivo"); }

  int32_t num_blocks;
  file.read(reinterpret_cast<char *>(&num_blocks), sizeof(num_blocks));

  std::vector<Block> blocks(num_blocks);
  for (Block & block : blocks) {
    file.read(reinterpret_cast<char *>(&block.num_particles), sizeof(block.num_particles));

    block.particles.resize(block.num_particles);
    for (Particle & particle : block.particles) {
      file.read(reinterpret_cast<char *>(&particle.id), sizeof(particle.id));
      file.read(reinterpret_cast<char *>(&particle.posx), sizeof(particle.posx));
      file.read(reinterpret_cast<char *>(&particle.posy), sizeof(particle.posy));
      file.read(reinterpret_cast<char *>(&particle.posz), sizeof(particle.posz));
      file.read(reinterpret_cast<char *>(&particle.hvx), sizeof(particle.hvx));
      file.read(reinterpret_cast<char *>(&particle.hvy), sizeof(particle.hvy));
      file.read(reinterpret_cast<char *>(&particle.hvz), sizeof(particle.hvz));
      file.read(reinterpret_cast<char *>(&particle.velx), sizeof(particle.velx));
      file.read(reinterpret_cast<char *>(&particle.vely), sizeof(particle.vely));
      file.read(reinterpret_cast<char *>(&particle.velz), sizeof(particle.velz));
      file.read(reinterpret_cast<char *>(&particle.density), sizeof(particle.density));
      file.read(reinterpret_cast<char *>(particle.acc), sizeof(particle.acc));
    }
  }

  return blocks;
}

void print_blocks(std::vector<Block> const & blocks) {
  for (size_t i = 0; i < blocks.size(); ++i) {
    std::cout << "Bloque " << i << " con " << blocks[i].num_particles << " particulas: ";
    // Ordena las partículas por id
    std::vector new_particles = blocks[i].particles;
    std::sort(new_particles.begin(), new_particles.end(),
              [](Particle const & a, Particle const & b) {
                return a.id < b.id;
              });
    for (Particle const & particle : new_particles) { std::cout << particle.id << " "; }
    std::cout << "\n";
  }
}

void print_particles(std::vector<Block> const & blocks) {
  // Recoge todas las partículas en un solo vector
  std::vector<Particle> particles;
  for (Block const & block : blocks) {
    particles.insert(particles.end(), block.particles.begin(), block.particles.end());
  }

  // Ordena las partículas por id
  std::sort(particles.begin(), particles.end(), [](Particle const & a, Particle const & b) {
    return a.id < b.id;
  });

  // Imprime las partículas
  for (Particle const & particle : particles) {
    std::cout << "ID: " << particle.id << "\n";
    std::cout << "Posicion: (" << particle.posx << ", " << particle.posy << ", " << particle.posz
              << ")\n";
    std::cout << "HV: (" << particle.hvx << ", " << particle.hvy << ", " << particle.hvz << ")\n";
    std::cout << "Velocidad: (" << particle.velx << ", " << particle.vely << ", " << particle.velz
              << ")\n";
    std::cout << "Densidad: " << particle.density << "\n";
    std::cout << "Acc: (" << particle.acc[0] << ", " << particle.acc[1] << ", " << particle.acc[2]
              << ")\n";
    std::cout << "\n";
  }
}

int main(int argc, char * argv[]) {
  if (argc != 2) {
    std::cerr << "Uso: " << argv[0] << " <archivo>\n";
    return 1;
  }

  try {
    std::vector<Block> blocks = read_file(argv[1]);
    print_particles(blocks);
  } catch (std::exception const & e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}