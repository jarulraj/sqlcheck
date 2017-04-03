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

struct NoOp {
    void operator()(...) const {}
};

void Check(const configuration& state) {

  std::shared_ptr<std::istream> input;

  // Set up stream
  if (state.file_name.empty()) {
    input.reset(&std::cin, NoOp());
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
      CheckStatement(sql_statement.str());

      // Reset statement
      sql_statement.str(std::string());
    }

  }

}

void CheckStatement(const std::string& sql_statement){

  std::cout << "SQL statement: " << sql_statement << "\n";

}

}  // namespace machine

