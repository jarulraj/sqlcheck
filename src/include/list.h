// LIST HEADER

#pragma once

#include "configuration.h"

namespace sqlcheck {

// LOGICAL DATABASE DESIGN

void CheckMultiValuedAttribute(const Configuration& state,
                               const std::string& sql_statement);

void CheckRecursiveDependency(const Configuration& state,
                              const std::string& sql_statement);

void CheckPrimaryKeyExists(const Configuration& state,
                           const std::string& sql_statement);

void CheckGenericPrimaryKey(const Configuration& state,
                            const std::string& sql_statement);

void CheckForeignKeyExists(const Configuration& state,
                           const std::string& sql_statement);

void CheckVariableAttribute(const Configuration& state,
                            const std::string& sql_statement);

void CheckMultiColumnAttribute(const Configuration& state,
                               const std::string& sql_statement);

void CheckMetadataTribbles(const Configuration& state,
                           const std::string& sql_statement);

// PHYSICAL DATABASE DESIGN

void CheckFloat(const Configuration& state,
                const std::string& sql_statement);


// QUERY

void CheckSelectStar(const Configuration& state,
                     const std::string& sql_statement);


}  // namespace machine
