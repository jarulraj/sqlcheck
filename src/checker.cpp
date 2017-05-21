// CHECKER SOURCE

#include <fstream>
#include <istream>
#include <sstream>
#include <string>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <functional>
#include <regex>

#include "checker.h"

#include "include/configuration.h"
#include "include/list.h"

namespace sqlcheck {

void Check(Configuration& state) {

  std::unique_ptr<std::istream> input;

  // Set up stream
  if(state.testing_mode == true){
    input.reset(state.test_stream.release());
  }
  else if (state.file_name.empty()) {
    input.reset(&std::cin);
  }
  else {
    std::cout << "Checking " << state.file_name << "...\n";
    input.reset(new std::ifstream(state.file_name.c_str()));
  }

  std::stringstream sql_statement;
  size_t fragment_size = 4096;
  char buffer[fragment_size];
  char delimiter = ';';

  // Go over the input stream
  while(!input->eof()){

    // Get a line from the input stream
    input->getline(buffer, fragment_size);
    std::string statement_fragment(buffer);

    // Append fragment to statement
    if(statement_fragment.empty() == false){
      sql_statement << statement_fragment << " ";
    }

    // Check for delimiter in line
    std::size_t location = statement_fragment.find(delimiter);
    if (location != std::string::npos) {

      // Check the statement
      CheckStatement(state, sql_statement.str());

      // Reset statement
      sql_statement.str(std::string());

    }

  }

}

void PrintMessage(const Configuration& state,
                  const std::string sql_statement,
                  const bool print_statement,
                  const LogLevel pattern_level,
                  const std::string title,
                  const std::string message){

  if(print_statement == true){
    std::cout << "\n-------------------------------------------------\n";
    std::cout << "SQL Statement:" << sql_statement << "\n";
  }

  if(state.file_name.empty() == false){
    std::cout << "[" << state.file_name << "]: ";
  }
  std::cout << "(" << LogLevelToString(pattern_level) << ") ";
  std::cout << title << "\n";

  // Print detailed message only in verbose mode
  if(state.verbose_mode == true){
    std::cout << message << "\n";
  }

}

void CheckPattern(const Configuration& state,
                  const std::string& sql_statement,
                  bool& print_statement,
                  const std::regex& anti_pattern,
                  const LogLevel pattern_level,
                  UNUSED_ATTRIBUTE const PatternType pattern_type,
                  const std::string title,
                  const std::string message,
                  const bool exists){

  //std::cout << "PATTERN LEVEL: " << pattern_level << "\n";
  //std::cout << "CHECKER LEVEL: " << state.log_level << "\n";

  // Check log level
  if(pattern_level < state.log_level){
    return;
  }

  bool found = false;

  try {
    std::sregex_iterator next(sql_statement.begin(),
                              sql_statement.end(),
                              anti_pattern);
    std::sregex_iterator end;
    while (next != end) {
      std::smatch match = *next;
      found = true;
      next++;
    }
  } catch (std::regex_error& e) {
    // Syntax error in the regular expression
  }

  if(found == exists){
    PrintMessage(state,
                 sql_statement,
                 print_statement,
                 pattern_level,
                 title,
                 message);

    // TOGGLE PRINT STATEMENT
    print_statement = false;
  }

}

void CheckStatement(const Configuration& state,
                    const std::string& sql_statement){

  // TRANSFORM TO LOWER CASE
  auto statement = sql_statement;

  std::transform(statement.begin(),
                 statement.end(),
                 statement.begin(),
                 ::tolower);

  // REMOVE SPACE
  statement = std::regex_replace(statement, std::regex("^ +| +$|( ) +"), "$1");

  // RESET
  bool print_statement = true;

  // LOGICAL DATABASE DESIGN

  CheckMultiValuedAttribute(state, statement, print_statement);

  CheckRecursiveDependency(state, statement, print_statement);

  CheckPrimaryKeyExists(state, statement, print_statement);

  CheckGenericPrimaryKey(state, statement, print_statement);

  CheckForeignKeyExists(state, statement, print_statement);

  CheckVariableAttribute(state, statement, print_statement);

  CheckMultiColumnAttribute(state, statement, print_statement);

  CheckMetadataTribbles(state, statement, print_statement);

  // PHYSICAL DATABASE DESIGN

  CheckFloat(state, statement, print_statement);

  // QUERY

  CheckSelectStar(state, statement, print_statement);

}

}  // namespace machine

