#ifndef PROGARGS_HPP
#define PROGARGS_HPP
#include <cstdint>
#include <string>
#include <vector>

namespace fluids::sim {
  struct Configuration {
      uint16_t nts;
      std::string inputFile;
      std::string outputFile;
  };

  class ProgramArguments {
    public:
      // Constructor
      int argc;
      std::vector<std::string> argv;
      ProgramArguments(int argc, std::vector<std::string> argv);
      // Métodos para acceder a los argumentos
      struct Configuration parseArguments();
      void correctArguments();

    private:
  };

  int CheckNSteps(std::string const & nts);
}  // namespace fluids::sim
#endif  // PROGARGS_HPP
