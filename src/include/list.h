// LIST HEADER

#pragma once

#include "configuration.h"

namespace sqlcheck {

// LOGICAL DATABASE DESIGN

void CheckMultiValuedAttribute(const Configuration& state,
                               const std::string& sql_statement,
                               bool& print_statement);

void CheckRecursiveDependency(const Configuration& state,
                              const std::string& sql_statement,
                              bool& print_statement);

void CheckPrimaryKeyExists(const Configuration& state,
                           const std::string& sql_statement,
                           bool& print_statement);

void CheckGenericPrimaryKey(const Configuration& state,
                            const std::string& sql_statement,
                            bool& print_statement);

void CheckForeignKeyExists(const Configuration& state,
                           const std::string& sql_statement,
                           bool& print_statement);

void CheckVariableAttribute(const Configuration& state,
                            const std::string& sql_statement,
                            bool& print_statement);

void CheckMetadataTribbles(const Configuration& state,
                           const std::string& sql_statement,
                           bool& print_statement);

// PHYSICAL DATABASE DESIGN

void CheckFloat(const Configuration& state,
                const std::string& sql_statement,
                bool& print_statement);

void CheckValuesInDefinition(const Configuration& state,
                             const std::string& sql_statement,
                             bool& print_statement);

void CheckExternalFiles(const Configuration& state,
                        const std::string& sql_statement,
                        bool& print_statement);

void CheckIndexCount(const Configuration& state,
                     const std::string& sql_statement,
                     bool& print_statement);

void CheckIndexAttributeOrder(const Configuration& state,
                              const std::string& sql_statement,
                              bool& print_statement);

// QUERY

void CheckSelectStar(const Configuration& state,
                     const std::string& sql_statement,
                     bool& print_statement);


}  // namespace machine
