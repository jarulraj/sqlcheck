// CHECKER HEADER

#pragma once

#include "configuration.h"

namespace sqlcheck {

// Check a set of SQL statements
void Check(configuration& state);

// Check a SQL statement
void CheckStatement(const configuration& state,
                    const std::string& sql_statement);

}  // namespace machine
