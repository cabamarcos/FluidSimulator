#include "sim/progargs.hpp"

#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>


using namespace std;
using namespace fluids::sim;

class ProgramArgumentsTest : public ::testing::Test {
  protected:
    void SetUp() override {
      ofstream input("input.txt");
      input << "1 2 3 4 5 6 7 8 9 10" << "\n";
      input.close();
      if (remove("output.txt") != 0) { }
    }

    void TearDown() override {
      if (remove("input.txt") != 0) { perror("Error deleting file"); }
    }
};

TEST_F(ProgramArgumentsTest, ParseArgumentsTest) {
  int const argc            = 4;
  vector<string> const argv = {"./fluid", "10", "input.txt", "output.txt"};
  ProgramArguments args(argc, argv);

  Configuration const config = args.parseArguments();

  ASSERT_EQ(config.nts, 10);
  ASSERT_EQ(config.inputFile, "input.txt");
  ASSERT_EQ(config.outputFile, "output.txt");
}

TEST_F(ProgramArgumentsTest, CheckNStepsTest) {
  string const validNts           = "10";
  string const invalidNts         = "abc";
  string const invalidNegativeNts = "-10";

  ASSERT_EQ(CheckNSteps(validNts), 0);
  ASSERT_EQ(CheckNSteps(invalidNts), -1);
  ASSERT_EQ(CheckNSteps(invalidNegativeNts), -2);
}

TEST_F(ProgramArgumentsTest, ArgumentsTest) {
  int const argc                    = 4;
  vector<string> const validArgs    = {"./fluid", "10", "input.txt", "output.txt"};
  vector<string> const invalidArgs1 = {"./fluid", "10", "input.txt", "output.txt", "extra"};
  vector<string> const invalidArgs2 = {"./fluid", "notanint", "input.txt", "output.txt"};
  vector<string> const invalidArgs3 = {"./fluid", "-10", "input.txt", "output.txt"};
  vector<string> const invalidArgs4 = {"./fluid", "10", "output.txt", "output.txt"};
  vector<string> const invalidArgs5 = {"./fluid", "10", "input.txt", "input.txt"};

  ProgramArguments const validArgsObj(argc, validArgs);
  ProgramArguments invalid_1ArgsObj(argc + 1, invalidArgs1);
  ProgramArguments invalid_2ArgsObj(argc, invalidArgs2);
  ProgramArguments invalid_3ArgsObj(argc, invalidArgs3);
  ProgramArguments invalid_4ArgsObj(argc, invalidArgs4);
  ProgramArguments invalid_5ArgsObj(argc, invalidArgs5);

  ASSERT_DEATH(invalid_1ArgsObj.correctArguments(), "Error: Invalid number of arguments: 4.\n");
  ASSERT_DEATH(invalid_2ArgsObj.correctArguments(), "Error: time steps must be numeric.\n");
  ASSERT_DEATH(invalid_3ArgsObj.correctArguments(), "Error: Invalid number of time steps.\n");
  ASSERT_DEATH(invalid_4ArgsObj.correctArguments(), "Error: Cannot open output.txt for reading.\n");
  ASSERT_DEATH(invalid_5ArgsObj.correctArguments(), "Error: Cannot open input.txt for writing.\n");
}

int main(int argc, char ** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}