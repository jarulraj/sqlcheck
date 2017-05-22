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
#include "include/color.h"

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

  ColorModifier red(ColorCode::FG_RED, state.color_mode, true);
  ColorModifier green(ColorCode::FG_GREEN, state.color_mode, true);
  ColorModifier blue(ColorCode::FG_BLUE, state.color_mode, true);
  ColorModifier regular(ColorCode::FG_DEFAULT, state.color_mode, false);

  if(print_statement == true){
    std::cout << "\n-------------------------------------------------\n";
    ColorModifier regular(ColorCode::FG_DEFAULT, state.color_mode, false);

    if(state.color_mode == true){
      std::cout << "SQL Statement: " << red << sql_statement << regular << "\n";
    }
    else {
      std::cout << "SQL Statement: " << sql_statement << "\n";
    }
  }

  if(state.color_mode == true){
    if(state.file_name.empty() == false){
      std::cout << "[" << state.file_name << "]: ";
    }

    std::cout << "(" << green << LogLevelToString(pattern_level) << regular << ") ";
    std::cout << blue << title << regular << "\n";
  }
  else {
    if(state.file_name.empty() == false){
      std::cout << "[" << state.file_name << "]: ";
    }

    std::cout << "(" << LogLevelToString(pattern_level) << ") ";
    std::cout << title << "\n";
  }

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
                  const bool exists,
                  const size_t min_count){

  //std::cout << "PATTERN LEVEL: " << pattern_level << "\n";
  //std::cout << "CHECKER LEVEL: " << state.log_level << "\n";

  // Check log level
  if(pattern_level < state.log_level){
    return;
  }

  bool found = false;
  std::smatch match;
  std::size_t count = 0;

  try {
    std::sregex_iterator next(sql_statement.begin(),
                              sql_statement.end(),
                              anti_pattern);
    std::sregex_iterator end;
    while (next != end) {
      match = *next;
      found = true;
      count++;
      next++;
    }
  } catch (std::regex_error& e) {
    // Syntax error in the regular expression
  }

  if(found == exists && count > min_count){
    PrintMessage(state,
                 sql_statement,
                 print_statement,
                 pattern_level,
                 title,
                 message);

    if(exists == true){
      ColorModifier blue(ColorCode::FG_BLUE, state.color_mode, true);
      ColorModifier regular(ColorCode::FG_DEFAULT, state.color_mode, false);

      if(state.color_mode == true){
        std::cout << "[Matching Expression: " << blue << match.str(0) << regular << "]\n";
      }
      else{
        std::cout << "[Matching Expression: " << match.str(0) << "]\n";
      }
    }

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

  CheckMetadataTribbles(state, statement, print_statement);

  // PHYSICAL DATABASE DESIGN

  CheckFloat(state, statement, print_statement);

  CheckValuesInDefinition(state, statement, print_statement);

  CheckExternalFiles(state, statement, print_statement);

  CheckIndexCount(state, statement, print_statement);

  CheckIndexAttributeOrder(state, statement, print_statement);

  // QUERY

  CheckSelectStar(state, statement, print_statement);

  CheckNullUsage(state, statement, print_statement);

  CheckNotNullUsage(state, statement, print_statement);

  CheckConcatenation(state, statement, print_statement);

  CheckGroupByUsage(state, statement, print_statement);

  CheckOrderByRand(state, statement, print_statement);

  CheckPatternMatching(state, statement, print_statement);

  CheckSpaghettiQuery(state, statement, print_statement);

  CheckJoinCount(state, statement, print_statement);

  CheckDistinctCount(state, statement, print_statement);

  CheckImplicitColumns(state, statement, print_statement);

}

}  // namespace machine

