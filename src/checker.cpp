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
#include <map>

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
    //std::cout << "Checking " << state.file_name << "...\n";
    input.reset(new std::ifstream(state.file_name.c_str()));
  }

  std::stringstream sql_statement;
  size_t fragment_size = 4096;
  char buffer[fragment_size];
  char delimiter = ';';

  std::cout << "==================== Results ===================\n";

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

  // Print summary
  if(state.checker_stats[RISK_LEVEL_ALL] == 0){
    std::cout << "No issues found.\n";
  }
  else {
    std::cout << "\n==================== Summary ===================\n";
    std::cout << "All Anti-Patterns  :: " << state.checker_stats[RISK_LEVEL_ALL] << "\n";
    std::cout << ">  High Risk   :: " << state.checker_stats[RISK_LEVEL_HIGH] << "\n";
    std::cout << ">  Medium Risk :: " << state.checker_stats[RISK_LEVEL_MEDIUM] << "\n";
    std::cout << ">  Low Risk    :: " << state.checker_stats[RISK_LEVEL_LOW] << "\n";
  }

}

// Wrap the text
std::string WrapText(const std::string& text){

  size_t line_length = 80;

  std::istringstream words(text);
  std::ostringstream wrapped;
  std::string word;
  bool newline = false;
  bool newpara = false;

  if (words >> word) {

    wrapped << word;

    size_t space_left = line_length - word.length();
    while (words >> word) {
      if(word == "●"){
        wrapped << "\n\n";
        newpara = true;
      }
      else{
        newpara = false;
      }

      if (space_left < word.length() + 1 || newline) {
        wrapped << '\n' << word;
        space_left = line_length - word.length();
      }
      else {
        if(newpara == false){
          wrapped << ' ' << word;
        }
        else{
          wrapped << word;
        }
        space_left -= word.length() + 1;
      }

      if(word.back() == ':'){
        newline = true;
      }
      else{
        newline = false;
      }
    }

  }

  return wrapped.str();
}

void PrintMessage(Configuration& state,
                  const std::string sql_statement,
                  const bool print_statement,
                  const RiskLevel pattern_risk_level,
                  const PatternType pattern_type,
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
      std::cout << "SQL Statement: " << red << WrapText(sql_statement) << regular << "\n";
    }
    else {
      std::cout << "SQL Statement: " << WrapText(sql_statement) << "\n";
    }
  }

  if(state.color_mode == true){
    if(state.file_name.empty() == false){
      std::cout << "[" << state.file_name << "]: ";
    }

    std::cout << "(" << green << RiskLevelToString(pattern_risk_level) << regular << ") ";
    std::cout << blue << title << regular << "\n";
  }
  else {
    if(state.file_name.empty() == false){
      std::cout << "[" << state.file_name << "]: ";
    }

    std::cout << "(" << RiskLevelToString(pattern_risk_level) << ") ";
    std::cout << "(" << PatternTypeToString(pattern_type) << ") ";
    std::cout << title << "\n";
  }

  // Print detailed message only in verbose mode
  if(state.verbose_mode == true){
    std::cout << WrapText(message) << "\n";
  }

  // Update checker stats
  state.checker_stats[pattern_risk_level]++;
  state.checker_stats[RISK_LEVEL_ALL]++;

}

void CheckPattern(Configuration& state,
                  const std::string& sql_statement,
                  bool& print_statement,
                  const std::regex& anti_pattern,
                  const RiskLevel pattern_risk_level,
                  const PatternType pattern_type,
                  const std::string title,
                  const std::string message,
                  const bool exists,
                  const size_t min_count){

  //std::cout << "PATTERN LEVEL: " << pattern_risk_level << "\n";
  //std::cout << "CHECKER LEVEL: " << state.log_level << "\n";

  // Check log level
  if(pattern_risk_level < state.risk_level){
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
                 pattern_risk_level,
                 pattern_type,
                 title,
                 message);

    if(exists == true){
      ColorModifier blue(ColorCode::FG_BLUE, state.color_mode, true);
      ColorModifier regular(ColorCode::FG_DEFAULT, state.color_mode, false);
      if(state.color_mode == true){
        std::cout << "[Matching Expression: " << blue << match.str(0) << regular << "]";
      }
      else{
        std::cout << "[Matching Expression: " << match.str(0) << "]";
      }
      std::cout << "\n\n";
    }

    // TOGGLE PRINT STATEMENT
    print_statement = false;
  }

}

void CheckStatement(Configuration& state,
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

  CheckHaving(state, statement, print_statement);

  CheckNesting(state, statement, print_statement);

  CheckOr(state, statement, print_statement);

  CheckUnion(state, statement, print_statement);

  CheckDistinctJoin(state, statement, print_statement);

  // APPLICATION

  CheckReadablePasswords(state, statement, print_statement);


}

}  // namespace machine

