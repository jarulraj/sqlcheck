// LIST HEADER

#pragma once

#include "configuration.h"

namespace sqlcheck {

// LOGICAL DATABASE DESIGN

void CheckMultiValuedAttribute(Configuration& state,
                               const std::string& sql_statement,
                               bool& print_statement);

void CheckRecursiveDependency(Configuration& state,
                              const std::string& sql_statement,
                              bool& print_statement);

void CheckPrimaryKeyExists(Configuration& state,
                           const std::string& sql_statement,
                           bool& print_statement);

void CheckGenericPrimaryKey(Configuration& state,
                            const std::string& sql_statement,
                            bool& print_statement);

void CheckForeignKeyExists(Configuration& state,
                           const std::string& sql_statement,
                           bool& print_statement);

void CheckVariableAttribute(Configuration& state,
                            const std::string& sql_statement,
                            bool& print_statement);

void CheckMetadataTribbles(Configuration& state,
                           const std::string& sql_statement,
                           bool& print_statement);

// PHYSICAL DATABASE DESIGN

void CheckFloat(Configuration& state,
                const std::string& sql_statement,
                bool& print_statement);

void CheckValuesInDefinition(Configuration& state,
                             const std::string& sql_statement,
                             bool& print_statement);

void CheckExternalFiles(Configuration& state,
                        const std::string& sql_statement,
                        bool& print_statement);

void CheckIndexCount(Configuration& state,
                     const std::string& sql_statement,
                     bool& print_statement);

void CheckIndexAttributeOrder(Configuration& state,
                              const std::string& sql_statement,
                              bool& print_statement);

// QUERY

void CheckSelectStar(Configuration& state,
                     const std::string& sql_statement,
                     bool& print_statement);

void CheckNullUsage(Configuration& state,
                    const std::string& sql_statement,
                    bool& print_statement);

void CheckNotNullUsage(Configuration& state,
                       const std::string& sql_statement,
                       bool& print_statement);

void CheckConcatenation(Configuration& state,
                        const std::string& sql_statement,
                        bool& print_statement);

void CheckGroupByUsage(Configuration& state,
                       const std::string& sql_statement,
                       bool& print_statement);

void CheckOrderByRand(Configuration& state,
                      const std::string& sql_statement,
                      bool& print_statement);

void CheckPatternMatching(Configuration& state,
                          const std::string& sql_statement,
                          bool& print_statement);

void CheckSpaghettiQuery(Configuration& state,
                         const std::string& sql_statement,
                         bool& print_statement);

void CheckJoinCount(Configuration& state,
                         const std::string& sql_statement,
                         bool& print_statement);

void CheckDistinctCount(Configuration& state,
                        const std::string& sql_statement,
                        bool& print_statement);

void CheckImplicitColumns(Configuration& state,
                          const std::string& sql_statement,
                          bool& print_statement);

void CheckHaving(Configuration& state,
                 const std::string& sql_statement,
                 bool& print_statement);

void CheckNesting(Configuration& state,
                  const std::string& sql_statement,
                  bool& print_statement);

void CheckOr(Configuration& state,
             const std::string& sql_statement,
             bool& print_statement);

void CheckUnion(Configuration& state,
                const std::string& sql_statement,
                bool& print_statement);

void CheckDistinctJoin(Configuration& state,
                       const std::string& sql_statement,
                       bool& print_statement);

// APPLICATION

void CheckReadablePasswords(Configuration& state,
                            const std::string& sql_statement,
                            bool& print_statement);


}  // namespace machine
