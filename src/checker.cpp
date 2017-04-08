// CHECKER SOURCE

#include <fstream>
#include <istream>
#include <sstream>
#include <string>
#include <memory>
#include <iostream>

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

void CheckSelectStar(const configuration& state,
                    const std::string& sql_statement){

  std::size_t found = sql_statement.find("SELECT *");
  if (found != std::string::npos) {
    std::cout << "SELECT * : Found at location " << found << " " << sql_statement << '\n';
  }

}

void CheckStatement(const configuration& state,
                    const std::string& sql_statement){

  std::cout << "SQL statement: " << sql_statement << "\n";

  CheckSelectStar(state, sql_statement);

}

}  // namespace machine

