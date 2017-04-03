// CONFIGURATION SOURCE

#include "configuration.h"

namespace sqlcheck {

void Usage() {
  std::cout <<
      "\n"
      "Command line options : sqlcheck <options>\n"
      "   -f --file_name                :  file name\n";
      "   -l --verbosity_level          :  verbosity level\n";
  exit(EXIT_FAILURE);
}

static struct option opts[] = {
    {"file_name", optional_argument, NULL, 'f'},
    {"verbosity_level", optional_argument, NULL, 'l'},
    {NULL, 0, NULL, 0}
};

std::string VerbosityLevelToString(const VerbosityLevel& verbosity_level){

  switch (verbosity_level) {
    case VERBOSITY_LEVEL_CRITICAL:
      return "CRITICAL";
    case VERBOSITY_LEVEL_WARNING:
      return "WARNING";
    case VERBOSITY_LEVEL_ALL:
      return "ALL";

    case VERBOSITY_LEVEL_INVALID:
    default:
      return "INVALID";
  }

}

static void ValidateVerbosityLevel(const configuration &state) {
  if (state.verbosity_level < 1 || state.verbosity_level > 3) {
    printf("Invalid verbosity_level :: %d\n", state.verbosity_level);
    exit(EXIT_FAILURE);
  }
  else {
    printf("--> %10s : %s\n", "VERBOSITY LEVEL",
           VerbosityLevelToString(state.verbosity_level).c_str());
  }
}

static void ValidateFileName(const configuration &state) {
  if (state.file_name.empty() == false) {
    printf("--> %10s : %s\n", "INPUT FILE NAME",
           state.file_name.c_str());
  }
}

void ParseArguments(int argc, char *argv[], configuration &state) {

  // Default Values
  state.verbosity_level = VERBOSITY_LEVEL_ALL;
  state.file_name = ""; // standard input

  // Parse args
  while (1) {
    int idx = 0;
    int c = getopt_long(argc, argv, "l:f:h",
                        opts, &idx);

    if (c == -1) break;

    switch (c) {
      case 'a':
        state.verbosity_level = (VerbosityLevel)atoi(optarg);
        break;
      case 'f':
        state.file_name = optarg;
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
  printf("--> %10s \n", "SQLCHECK 0.1");

  ValidateVerbosityLevel(state);
  ValidateFileName(state);

}

}  // namespace sqlcheck
