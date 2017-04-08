// CHECKER HEADER

#pragma once

#include "configuration.h"

namespace sqlcheck {

// Check a set of SQL statements
void Check(Configuration& state);

// Check a SQL statement
void CheckStatement(const Configuration& state,
                    const std::string& sql_statement);

}  // namespace machine
