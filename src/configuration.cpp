// CONFIGURATION SOURCE

#include "include/configuration.h"

namespace sqlcheck {

void Usage() {
  std::cout <<
      "\n"
      "Command line options : sqlcheck <options>\n"
      "   -f --file_name          :  file name\n"
      "   -r --risk_level         :  set of anti-patterns to check\n"
      "                           :  1 (all anti-patterns, default) \n"
      "                           :  2 (only medium and high risk anti-patterns) \n"
      "                           :  3 (only high risk anti-patterns) \n"
      "   -c --color_mode         :  color mode \n"
      "   -v --verbose_mode       :  verbose mode \n";
  exit(EXIT_SUCCESS);
}

static struct option opts[] = {
    {"color_mode", optional_argument, NULL, 'c'},
    {"file_name", optional_argument, NULL, 'f'},
    {"risk_level", optional_argument, NULL, 'r'},
    {"verbose_mode", optional_argument, NULL, 'v'},
    {NULL, 0, NULL, 0}
};

std::string RiskLevelToString(const RiskLevel& log_level){

  switch (log_level) {
    case RISK_LEVEL_HIGH:
      return "HIGH RISK";
    case RISK_LEVEL_MEDIUM:
      return "MEDIUM RISK";
    case RISK_LEVEL_LOW:
      return "LOW RISK";
    case RISK_LEVEL_ALL:
      return "ALL ANTI-PATTERNS";

    case RISK_LEVEL_INVALID:
    default:
      return "INVALID";
  }

}

std::string RiskLevelToDetailedString(const RiskLevel& log_level){

  switch (log_level) {
    case RISK_LEVEL_HIGH:
      return "ONLY HIGH RISK ANTI-PATTERNS";
    case RISK_LEVEL_MEDIUM:
      return "ONLY MEDIUM AND HIGH RISK ANTI-PATTERNS";
    case RISK_LEVEL_LOW:
      return "ALL ANTI-PATTERNS";
    case RISK_LEVEL_ALL:
      return "ALL ANTI-PATTERNS";

    case RISK_LEVEL_INVALID:
    default:
      return "INVALID";
  }

}


std::string PatternTypeToString(const PatternType& pattern_type){

  switch (pattern_type) {
    case PATTERN_TYPE_PHYSICAL_DATABASE_DESIGN:
      return "PHYSICAL_DATABASE_DESIGN";
    case PATTERN_TYPE_LOGICAL_DATABASE_DESIGN:
      return "LOGICAL_DATABASE_DESIGN";
    case PATTERN_TYPE_QUERY:
      return "QUERY";
    case PATTERN_TYPE_APPLICATION:
      return "APPLICATION";

    case PATTERN_TYPE_INVALID:
    default:
      return "INVALID";
  }

}

static void ValidateRiskLevel(const Configuration &state) {
  if (state.risk_level < RISK_LEVEL_ALL || state.risk_level > RISK_LEVEL_HIGH) {
    printf("INVALID RISK LEVEL :: %d\n", state.risk_level);
    exit(EXIT_FAILURE);
  }
  else {
    printf("> %s :: %s\n", "RISK LEVEL   ",
           RiskLevelToDetailedString(state.risk_level).c_str());
  }
}

static void ValidateFileName(const Configuration &state) {
  if (state.file_name.empty() == false) {
    printf("> %s :: %s\n", "SQL FILE NAME",
           state.file_name.c_str());
  }
}

void ParseArguments(int argc, char *argv[], Configuration &state) {

  // Default Values
  state.risk_level = RISK_LEVEL_ALL;
  state.file_name = ""; // standard input
  state.testing_mode = false;
  state.verbose_mode = false;
  state.color_mode = false;

  // Parse args
  while (1) {
    int idx = 0;
    int c = getopt_long(argc, argv, "f:cr:vh",
                        opts, &idx);

    if (c == -1) break;

    switch (c) {
      case 'c':
        state.color_mode = true;
        break;
      case 'f':
        state.file_name = std::string(optarg);
        break;
      case 'r':
        state.risk_level = (RiskLevel)atoi(optarg);
        break;
      case 'v':
        state.verbose_mode = true;
        break;

      case 'h':
        Usage();
        break;

      default:
        printf("Unknown option: -%c-\n", c);
        Usage();
    }
  }

  // Run validators
  std::cout << "-------------------------------------------------\n";
  ValidateRiskLevel(state);
  ValidateFileName(state);
  std::cout << "-------------------------------------------------\n";

}

}  // namespace sqlcheck
