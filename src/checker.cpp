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

#include "include/checker.h"

#include "include/configuration.h"
#include "include/list.h"
#include "include/color.h"

namespace sqlcheck {

bool Check(Configuration& state) {

  bool has_issues = false;
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
  state.line_number = 1;

  std::cout << "==================== Results ===================\n";

  // Go over the input stream
  while(!input->eof()){

    // Get a statement from the input stream
    std::string statement_fragment;
    std::getline(*input, statement_fragment, state.delimiter[0]);

    // Append fragment to statement
    if(statement_fragment.empty() == false){
      sql_statement << statement_fragment << " ";
    }

    // Check the statement
    CheckStatement(state, sql_statement.str());

    // Reset statement
    sql_statement.str(std::string());
  }

  // Print summary
  if(state.checker_stats[RISK_LEVEL_ALL] == 0){
    std::cout << "No issues found.\n";
  }
  else {
    std::cout << "\n==================== Summary ===================\n";
    std::cout << "All Anti-Patterns and Hints  :: " << state.checker_stats[RISK_LEVEL_ALL] << "\n";
    std::cout << ">  High Risk   :: " << state.checker_stats[RISK_LEVEL_HIGH] << "\n";
    std::cout << ">  Medium Risk :: " << state.checker_stats[RISK_LEVEL_MEDIUM] << "\n";
    std::cout << ">  Low Risk    :: " << state.checker_stats[RISK_LEVEL_LOW] << "\n";
    std::cout << ">  Hints       :: " << state.checker_stats[RISK_LEVEL_NONE] << "\n";
    has_issues = true;
  }

  // Skip destroying std::cin
  if (state.file_name.empty()) {
    input.release();
  }

  return has_issues;

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
      std::cout << "SQL Statement at line " << state.line_number <<": " << red << WrapText(sql_statement) << state.delimiter << regular << "\n";
    }
    else {
      std::cout << "SQL Statement at line " << state.line_number << ": " << WrapText(sql_statement) << state.delimiter << "\n";
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
  if(state.verbose == true){
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

  // create an vector for the match positions
  std::vector<size_t> positions;
  try {
    std::sregex_iterator sqlsearch = std::sregex_iterator(sql_statement.begin(), sql_statement.end(), anti_pattern);
    std::sregex_iterator sqlend = std::sregex_iterator();
    count = std::distance(sqlsearch, sqlend);
    if (count > 0) {
      found = true;
    }

    if(found == exists && count > min_count){
      for (std::sregex_iterator next = sqlsearch; next != sqlend; ++next)
      {
          match = *next;
          // add match position to the vector
          positions.push_back(match.position(0));
      }

      // update positions from character number to line number
      uint32_t position_checker = 0;
      uint32_t num_lines = state.line_number;
      if (positions.size() > 0) {
        for (size_t statement_char = 0; statement_char < sql_statement.length(); statement_char++) {
          if (positions[position_checker] == statement_char) {
            positions[position_checker] = num_lines;
            position_checker++;
          }
          if (sql_statement[statement_char] == '\n') {
            num_lines++;
          }
        }
      }

      std::stringstream linelocations;
      // convert line numbers to output string
      if (positions.size() > 1) {
        linelocations << " at lines ";
      } else {
        linelocations << " at line ";
      }
      for (size_t i = 0; i < positions.size(); i++) {
          linelocations << positions[i];
          if (i < positions.size() - 1) {
              linelocations << ", ";
          }
      }
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
          std::cout << "[Matching Expression: " << blue << WrapText(match.str(0)) << regular << linelocations.str()  << "]";
        }
        else{
          std::cout << "[Matching Expression: " << WrapText(match.str(0)) << linelocations.str() << "]";
        }
        std::cout << "\n\n";
      }

      // TOGGLE PRINT STATEMENT
      print_statement = false;
    }
  } catch (std::regex_error& e) {
    // Syntax error in the regular expression
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

  // CHECK FOR LEADING NEWLINE
  if (statement[0] == '\n') {
    statement = statement.erase(0,1);
    state.line_number++;
  }

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

  CheckJoinWithoutEquality(state, statement, print_statement);

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




  // update state.line_number with number of line breaks in the statement that was just checked
  for (size_t i = 0; i < statement.length(); i++)
  {
      if (statement[i] == '\n')
      {
          state.line_number++;
      }
  }
}

}  // namespace machine

