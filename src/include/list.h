// LIST HEADER

#pragma once

#include "configuration.h"

namespace sqlcheck {

// QUERY

void CheckSelectStar(const Configuration& state,
                     const std::string& sql_statement);

// CREATION

void CheckMultiValuedAttribute(const Configuration& state,
                               const std::string& sql_statement);

void CheckRecursiveDependency(const Configuration& state,
                              const std::string& sql_statement);

}  // namespace machine
