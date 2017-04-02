// CONFIGURATION SOURCE

#include "configuration.h"

namespace sqlcheck {

void Usage() {
  std::cout <<
      "\n"
      "Command line options : sqlcheck <options>\n"
      "   -l --verbosity_level                :  verbosity level\n";
  exit(EXIT_FAILURE);
}

static struct option opts[] = {
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
    printf("%30s : %s\n", "caching_type",
           VerbosityLevelToString(state.verbosity_level).c_str());
  }
}

void ParseArguments(int argc, char *argv[], configuration &state) {

  // Default Values
  state.verbosity_level = VERBOSITY_LEVEL_ALL;

  // Parse args
  while (1) {
    int idx = 0;
    int c = getopt_long(argc, argv,
                        "l:h",
                        opts, &idx);

    if (c == -1) break;

    switch (c) {
      case 'a':
        state.verbosity_level = (VerbosityLevel)atoi(optarg);
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
  printf("//===----------------------------------------------------------------------===//\n");
  printf("//                               SQLCHECK                                      //\n");
  printf("//===----------------------------------------------------------------------===//\n");

  ValidateVerbosityLevel(state);

  printf("//===----------------------------------------------------------------------===//\n");

}

}  // namespace sqlcheck
