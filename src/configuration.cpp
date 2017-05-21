// CONFIGURATION SOURCE

#include "include/configuration.h"

namespace sqlcheck {

void Usage() {
  std::cout <<
      "\n"
      "Command line options : sqlcheck <options>\n"
      "   -c --color_mode         :  color mode \n"
      "   -f --file_name          :  file name\n"
      "   -l --log_level          :  log level\n"
      "   -v --verbose_mode       :  verbose mode \n";
  exit(EXIT_SUCCESS);
}

static struct option opts[] = {
    {"color_mode", optional_argument, NULL, 'c'},
    {"file_name", optional_argument, NULL, 'f'},
    {"log_level", optional_argument, NULL, 'l'},
    {"verbose_mode", optional_argument, NULL, 'v'},
    {NULL, 0, NULL, 0}
};

std::string LogLevelToString(const LogLevel& log_level){

  switch (log_level) {
    case LOG_LEVEL_ERROR:
      return "ERROR";
    case LOG_LEVEL_WARN:
      return "WARN";
    case LOG_LEVEL_INFO:
      return "INFO";
    case LOG_LEVEL_DEBUG:
      return "DEBUG";
    case LOG_LEVEL_TRACE:
      return "TRACE";
    case LOG_LEVEL_ALL:
      return "ALL";

    case LOG_LEVEL_INVALID:
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

static void ValidateLogLevel(const Configuration &state) {
  if (state.log_level < LOG_LEVEL_ALL || state.log_level > LOG_LEVEL_INVALID) {
    printf("Invalid log_level :: %d\n", state.log_level);
    exit(EXIT_FAILURE);
  }
  else {
    printf("--> %s : %s\n", "LOG LEVEL",
           LogLevelToString(state.log_level).c_str());
  }
}

static void ValidateFileName(const Configuration &state) {
  if (state.file_name.empty() == false) {
    printf("--> %s : %s\n", "INPUT FILE NAME",
           state.file_name.c_str());
  }
}

void ParseArguments(int argc, char *argv[], Configuration &state) {

  // Default Values
  state.log_level = LOG_LEVEL_ALL;
  state.file_name = ""; // standard input
  state.testing_mode = false;
  state.verbose_mode = false;
  state.color_mode = true;

  // Parse args
  while (1) {
    int idx = 0;
    int c = getopt_long(argc, argv, "l:f:cvh",
                        opts, &idx);

    if (c == -1) break;

    switch (c) {
      case 'c':
        state.color_mode = false;
        break;
      case 'f':
        state.file_name = optarg;
        break;
      case 'l':
        state.log_level = (LogLevel)atoi(optarg);
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
  printf("--> %s \n", "SQLCHECK 0.1");

  ValidateLogLevel(state);
  ValidateFileName(state);

}

}  // namespace sqlcheck
