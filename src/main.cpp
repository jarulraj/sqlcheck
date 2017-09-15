// MAIN SOURCE

#include <iostream>
#include <fstream>

#include "checker.h"
#include "include/configuration.h"

#include "gflags/gflags.h"

namespace sqlcheck {

Configuration state;

}  // namespace sqlcheck

DEFINE_bool(c, false, "Display warnings in color mode");
DEFINE_bool(v, false, "Display verbose warnings");
DEFINE_uint64(r, sqlcheck::RISK_LEVEL_ALL,
              "Set of anti-patterns to check \n"
              "1 (all anti-patterns, default) \n"
              "2 (only medium and high risk anti-patterns) \n"
              "3 (only high risk anti-patterns) \n");
DEFINE_string(f, "", "SQL file name"); // standard input

void ConfigureChecker(sqlcheck::Configuration &state) {

  // Default Values
  state.risk_level = sqlcheck::RISK_LEVEL_ALL;
  state.file_name = "";
  state.testing_mode = false;
  state.verbose = false;
  state.color_mode = false;

  // Configure checker
  state.color_mode = FLAGS_c;
  state.verbose = FLAGS_v;
  state.file_name = FLAGS_f;
  state.risk_level = (sqlcheck::RiskLevel) FLAGS_r;

  // Run validators
  std::cout << "+-------------------------------------------------+\n"
            << "|                   SQLCHECK                      |\n"
            << "+-------------------------------------------------+\n";

  ValidateRiskLevel(state);
  ValidateFileName(state);
  std::cout << "-------------------------------------------------\n";

}

int main(int argc, char **argv) {

  try {

    // Parse the input arguments from the user
    gflags::SetVersionString("1.2");

    gflags::ParseCommandLineFlags(&argc, &argv, true);

    // Customize the checker configuration
    ConfigureChecker(sqlcheck::state);

    // Invoke the checker
    sqlcheck::Check(sqlcheck::state);

  }
  // Catching at the top level ensures that
  // destructors are always called
  catch (std::exception& exc) {
    std::cerr << exc.what() << std::endl;
    gflags::ShutDownCommandLineFlags();
    exit(EXIT_FAILURE);
  }

  gflags::ShutDownCommandLineFlags();
  return (EXIT_SUCCESS);
}
