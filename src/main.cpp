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
DEFINE_bool(color_mode, false, "Display warnings in color mode");
DEFINE_bool(v, false, "Display verbose warnings");
DEFINE_bool(verbose, false, "Display verbose warnings");
DEFINE_string(d, "", "Query delimiter string (default -- ;)");
DEFINE_string(delimiter, "", "Query delimiter string (default -- ;)");
DEFINE_bool(h, false, "Print help message");
DEFINE_uint64(r, sqlcheck::RISK_LEVEL_ALL,
              "Set of anti-patterns to check \n"
              "1 (all anti-patterns, default) \n"
              "2 (only medium and high risk anti-patterns) \n"
              "3 (only high risk anti-patterns) \n");
DEFINE_uint64(risk_level, sqlcheck::RISK_LEVEL_ALL,
              "Set of anti-patterns to check \n"
              "1 (all anti-patterns, default) \n"
              "2 (only medium and high risk anti-patterns) \n"
              "3 (only high risk anti-patterns) \n");
DEFINE_string(f, "", "SQL file name"); // standard input
DEFINE_string(file_name, "", "SQL file name"); // standard input

void ConfigureChecker(sqlcheck::Configuration &state) {

  // Default Values
  state.risk_level = sqlcheck::RISK_LEVEL_ALL;
  state.file_name = "";
  state.delimiter = ";";
  state.testing_mode = false;
  state.verbose = false;
  state.color_mode = false;

  // Configure checker
  state.color_mode = FLAGS_c || FLAGS_color_mode;
  state.verbose = FLAGS_v || FLAGS_verbose;
  if(FLAGS_f.empty() == false){
    state.file_name = FLAGS_f;
  }
  if(FLAGS_file_name.empty() == false){
    state.file_name = FLAGS_file_name;
  }
  if(FLAGS_d.empty() == false){
    state.delimiter = FLAGS_f;
  }
  if(FLAGS_delimiter.empty() == false){
    state.delimiter = FLAGS_delimiter;
  }
  if(FLAGS_r != 0){
    state.risk_level = (sqlcheck::RiskLevel) FLAGS_r;
  }
  if(FLAGS_risk_level != 0){
    state.risk_level = (sqlcheck::RiskLevel) FLAGS_risk_level;
  }

  // Run validators
  std::cout << "+-------------------------------------------------+\n"
            << "|                   SQLCHECK                      |\n"
            << "+-------------------------------------------------+\n";

  ValidateRiskLevel(state);
  ValidateFileName(state);
  ValidateColorMode(state);
  ValidateVerbose(state);
  ValidateDelimiter(state);

  std::cout << "-------------------------------------------------\n";

}

void Usage() {
  std::cout <<
      "Command line options : sqlcheck <options>\n"
      "   -f -file_name          :  SQL file name\n"
      "   -r -risk_level         :  Set of anti-patterns to check\n"
      "                          :  1 (all anti-patterns, default) \n"
      "                          :  2 (only medium and high risk anti-patterns) \n"
      "                          :  3 (only high risk anti-patterns) \n"
      "   -c -color_mode         :  Display warnings in color mode \n"
      "   -v -verbose            :  Display verbose warnings \n"
      "   -d -delimiter          :  Query delimiter string (; by default) \n"
      "   -h -help               :  Print help message \n";
}

int main(int argc, char **argv) {

  bool has_issues = false;

  try {

    // Parse the input arguments from the user
    gflags::SetUsageMessage("");
    gflags::SetVersionString("1.2.1");

    gflags::ParseCommandLineFlags(&argc, &argv, true);

    // Print help message
    if(FLAGS_h == true){
      FLAGS_h = false;
      Usage();

      gflags::ShutDownCommandLineFlags();
      return (EXIT_SUCCESS);
    }

    // Customize the checker configuration
    ConfigureChecker(sqlcheck::state);

    // Invoke the checker
    has_issues = sqlcheck::Check(sqlcheck::state);

  }
  // Catching at the top level ensures that
  // destructors are always called
  catch (std::exception& exc) {
    std::cerr << exc.what() << std::endl;
    gflags::ShutDownCommandLineFlags();
    exit(EXIT_FAILURE);
  }

  gflags::ShutDownCommandLineFlags();

  (has_issues) ? exit(EXIT_FAILURE) : exit(EXIT_SUCCESS);
}
