#include <iostream>
#include "sim/progargs.hpp"
#include "sim/grid.hpp"
#include "sim/utils.hpp"

using namespace fluids::sim;

int main(int argc, char* argv[]) {
    std::vector<std::string> args(argv, argv + argc);
    ProgramArguments progargs = ProgramArguments(argc, args);
    progargs.correctArguments();
    struct Configuration config = progargs.parseArguments();
    struct File file = readFile(config.inputFile);
    Grid grid(file);
    for (uint16_t time_step = 0; time_step < config.nts; time_step++) {
        grid.makeSimulation();
    }
    grid.writeSimulation(config.outputFile);
    return 0;
}


