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
#include <map>

namespace sqlcheck {

#define UNUSED_ATTRIBUTE __attribute__((unused))

enum RiskLevel {
  RISK_LEVEL_INVALID = 10,

  RISK_LEVEL_HIGH = 3,
  RISK_LEVEL_MEDIUM = 2,
  RISK_LEVEL_LOW = 1,
  RISK_LEVEL_ALL = 0

};

enum PatternType {
  PATTERN_TYPE_INVALID = 0,

  PATTERN_TYPE_PHYSICAL_DATABASE_DESIGN = 1,
  PATTERN_TYPE_LOGICAL_DATABASE_DESIGN = 2,
  PATTERN_TYPE_QUERY = 3,
  PATTERN_TYPE_APPLICATION = 4,

};

// Checker stats
struct CheckerStats {

  unsigned int error_level_counter = 0;
  unsigned int warn_level_counter = 0;
  unsigned int info_level_counter = 0;
  unsigned int debug_level_counter = 0;
  unsigned int trace_level_counter = 0;
  unsigned int all_level_counter = 0;

};

class Configuration {
 public:

  // Constructor
  Configuration()
   :
     color_mode(true),
     file_name(""),
     risk_level(RiskLevel::RISK_LEVEL_ALL),
     verbose_mode(false),
     testing_mode(false) {
  }

  // color mode
  bool color_mode;

  // filename
  std::string file_name;

  // risk level
  RiskLevel risk_level;

  // verbose mode
  bool verbose_mode;

  // test stream
  std::unique_ptr<std::istringstream> test_stream;

  // testing mode
  bool testing_mode;

  /// checker stats
  std::map<int, int> checker_stats;

};

std::string RiskLevelToString(const RiskLevel& log_level);

std::string RiskLevelToDetailedString(const RiskLevel& log_level);

void Usage(FILE *out);

void ParseArguments(int argc, char *argv[], Configuration &state);

}  // namespace sqlcheck
