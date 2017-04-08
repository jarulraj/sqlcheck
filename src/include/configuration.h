// CONFIGURATION HEADER

#pragma once

#include <cstdlib>
#include <cstdio>
#include <getopt.h>
#include <sys/time.h>
#include <iostream>
#include <string>
#include <sstream>
#include <memory>

namespace sqlcheck {

enum LogLevel {
  LOG_LEVEL_INVALID = 10,

  LOG_LEVEL_ERROR = 5,
  LOG_LEVEL_WARN = 4,
  LOG_LEVEL_INFO = 3,
  LOG_LEVEL_DEBUG = 2,
  LOG_LEVEL_TRACE = 1,
  LOG_LEVEL_ALL = 0

};

class configuration {
 public:

  // log level
  LogLevel log_level;

  // filename
  std::string file_name;

  // test stream
  std::unique_ptr<std::istringstream> test_stream;

  // testing mode
  bool testing_mode;

};

std::string LogLevelToString(const LogLevel& log_level);

void Usage(FILE *out);

void ParseArguments(int argc, char *argv[], configuration &state);

}  // namespace sqlcheck
