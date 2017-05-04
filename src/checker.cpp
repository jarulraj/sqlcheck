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

void PrintMessage(const std::string file_name,
                  size_t offset,
                  const std::string sql_statement,
                  const LogLevel pattern_level,
                  const std::string title,
                  const std::string message){

  std::cout << "\nSQL Statement: " << sql_statement << "\n";
  std::cout << "[" << file_name << ":" << offset << "]: ";
  std::cout << "(" << LogLevelToString(pattern_level) << ") ";
  std::cout << title << " :: \n" << message << "\n";

}

void CheckPattern(const Configuration& state,
                  const std::string& sql_statement,
                  const std::string& anti_pattern,
                  const LogLevel pattern_level,
                  UNUSED_ATTRIBUTE const PatternType pattern_type,
                  const std::string title,
                  const std::string message){

  //std::cout << "PATTERN LEVEL: " << pattern_level << "\n";
  //std::cout << "CHECKER LEVEL: " << state.log_level << "\n";

  // Check log level
  if(pattern_level < state.log_level){
    return;
  }

  std::size_t offset = sql_statement.find(anti_pattern);
  if (offset != std::string::npos) {
    std::cout << "FOUND \n";

    PrintMessage(state.file_name,
                 offset,
                 sql_statement,
                 pattern_level,
                 title,
                 message);
  }

}

void CheckStatement(const Configuration& state,
                    const std::string& sql_statement){

  CheckSelectStar(state, sql_statement);

}

}  // namespace machine

