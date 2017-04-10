// LIST HEADER

#pragma once

#include "configuration.h"

namespace sqlcheck {

// Check for select star
void CheckSelectStar(const Configuration& state,
                     const std::string& sql_statement);

}  // namespace machine
