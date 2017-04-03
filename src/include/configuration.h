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

enum VerbosityLevel {
  VERBOSITY_LEVEL_INVALID = 0,

  VERBOSITY_LEVEL_CRITICAL = 1,
  VERBOSITY_LEVEL_WARNING = 2,
  VERBOSITY_LEVEL_ALL = 3

};

class configuration {
 public:

  // verbosity level
  VerbosityLevel verbosity_level;

  // filename
  std::string file_name;

  // test stream
  std::unique_ptr<std::istringstream> test_stream;

  // testing mode
  bool testing_mode;

};

void Usage(FILE *out);

void ParseArguments(int argc, char *argv[], configuration &state);

}  // namespace sqlcheck
