// CHECKER HEADER

#pragma once

#include <regex>

#include "configuration.h"

namespace sqlcheck {

// Check a set of SQL statements
bool Check(Configuration& state);

// Check a SQL statement
void CheckStatement(Configuration& state,
                    const std::string& sql_statement);

// Check a pattern
void CheckPattern(Configuration& state,
                  const std::string& sql_statement,
                  bool& print_statement,
                  const std::regex& anti_pattern,
                  const RiskLevel pattern_level,
                  const PatternType pattern_type,
                  const std::string title,
                  const std::string message,
                  const bool exists,
                  const size_t min_count = 0);

}  // namespace machine
