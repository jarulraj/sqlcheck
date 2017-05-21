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

#define UNUSED_ATTRIBUTE __attribute__((unused))

enum LogLevel {
  LOG_LEVEL_INVALID = 10,

  LOG_LEVEL_ERROR = 5,
  LOG_LEVEL_WARN = 4,
  LOG_LEVEL_INFO = 3,
  LOG_LEVEL_DEBUG = 2,
  LOG_LEVEL_TRACE = 1,
  LOG_LEVEL_ALL = 0

};

enum PatternType {
  PATTERN_TYPE_INVALID = 0,

  PATTERN_TYPE_PHYSICAL_DATABASE_DESIGN = 1,
  PATTERN_TYPE_LOGICAL_DATABASE_DESIGN = 2,
  PATTERN_TYPE_QUERY = 3,
  PATTERN_TYPE_APPLICATION = 4,

};

class Configuration {
 public:

  // Constructor
  Configuration()
   :
     color_mode(true),
     file_name(""),
     log_level(LogLevel::LOG_LEVEL_ALL),
     verbose_mode(false),
     testing_mode(false) {
  }

  // color mode
  bool color_mode;

  // filename
  std::string file_name;

  // log level
  LogLevel log_level;

  // verbose mode
  bool verbose_mode;

  // test stream
  std::unique_ptr<std::istringstream> test_stream;

  // testing mode
  bool testing_mode;

};

std::string LogLevelToString(const LogLevel& log_level);

void Usage(FILE *out);

void ParseArguments(int argc, char *argv[], Configuration &state);

}  // namespace sqlcheck
