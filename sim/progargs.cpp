#include "progargs.hpp"

#include "utils.hpp"

#include <fstream>
#include <iostream>
#include <utility>

namespace fluids::sim {

  ProgramArguments::ProgramArguments(int argc, std::vector<std::string> argv)
    : argc(argc), argv(std::move(argv)) { }

  struct Configuration ProgramArguments::parseArguments() {
    struct Configuration config { };

    config.nts        = static_cast<uint16_t>(std::stoi(argv[1]));
    config.inputFile  = argv[2];
    config.outputFile = argv[3];

    return config;
  }

  int CheckNSteps(std::string const & nts) {
    try {
      if (std::stoi(nts) <= 0) {
        std::cerr << "Error: Invalid number of time steps.\n";
        return ERROR_INVALID_NUMBER_TIME_STEPS;
      }
    } catch (std::invalid_argument const & ia) {
      std::cerr << "Error: time steps must be numeric.\n";
      return ERROR_INVALID_TIME_STEPS;
    }
    return 0;
  }

  void ProgramArguments::correctArguments() {
    if (this->argc != 4) {
      std::cerr << "Error: Invalid number of arguments: " << this->argc - 1 << ".\n";
      exit(ERROR_INVALID_NUMBER_ARGUMENTS);
    }
    int const status = CheckNSteps(this->argv[1]);
    if (status != 0) { exit(status); }
    std::ifstream inputFile(this->argv[2]);
    if (!inputFile.good()) {
      std::cerr << "Error: Cannot open " << this->argv[2] << " for reading.\n";
      exit(ERROR_CANNOT_OPEN_INPUT_FILE);
    }
    inputFile.close();
    // Check that output file doesn't exist
    std::ifstream outputFile(this->argv[3]);
    if (outputFile.good()) {
      std::cerr << "Error: Cannot open " << this->argv[3] << " for writing.\n";
      exit(ERROR_CANNOT_OPEN_OUTPUT_FILE);
    }
    outputFile.close();
  }
}  // namespace fluids::sim