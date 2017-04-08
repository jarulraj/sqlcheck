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
#include "configuration.h"

namespace sqlcheck {

void Check(configuration& state) {

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

void CheckPattern(const configuration& state,
                  const std::string& sql_statement,
                  const std::string& anti_pattern,
                  const LogLevel pattern_level,
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

void CheckSelectStar(const configuration& state,
                     const std::string& sql_statement){

  std::string pattern = "SELECT *";
  std::string title = "SELECT *";

  std::string message1 =
      "1) Inefficiency in moving data to the consumer:\n"
      "When you SELECT *, you're often retrieving more columns from the database than\n"
      "your application really needs to function. This causes more data to move from\n"
      "the database server to the client, slowing access and increasing load on your\n"
      "machines, as well as taking more time to travel across the network. This is\n"
      "especially true when someone adds new columns to underlying tables that didn't\n"
      "exist and weren't needed when the original consumers coded their data access.\n\n";

  std::string message2 =
      "2) Indexing issues:\n"
      "Consider a scenario where you want to tune a query to a high level of performance.\n"
      "If you were to use *, and it returned more columns than you actually needed,\n"
      "the server would often have to perform more expensive methods to retrieve your \n"
      "data than it otherwise might. For example, you wouldn't be able to create an index\n"
      "which simply covered the columns in your SELECT list, and even if you did \n"
      "(including all columns [shudder]), the next guy who came around and added a column\n"
      "to the underlying table would cause the optimizer to ignore your optimized covering\n"
      "index, and you'd likely find that the performance of your query would drop \n"
      "substantially for no readily apparent reason.\n\n";

  std::string message3 =
      "3) Binding Problems:\n"
      "When you SELECT *, it's possible to retrieve two columns of the same name from two\n"
      "different tables. This can often crash your data consumer. Imagine a query that joins\n"
      "two tables, both of which contain a column called \"ID\". How would a consumer know\n"
      "which was which? SELECT * can also confuse views (at least in some versions SQL Server)\n"
      "when underlying table structures change -- the view is not rebuilt, and the data which\n"
      "comes back can be nonsense. And the worst part of it is that you can take care to name\n"
      "your columns whatever you want, but the next guy who comes along might have no way of\n"
      "knowing that he has to worry about adding a column which will collide with your\n"
      "already-developed names.\n\n";

  CheckPattern(state,
               sql_statement,
               pattern,
               LOG_LEVEL_ERROR,
               title,
               message1 + message2 + message3);

}

void CheckStatement(const configuration& state,
                    const std::string& sql_statement){

  CheckSelectStar(state, sql_statement);

}

}  // namespace machine

