// MAIN SOURCE

#include <iostream>
#include <fstream>

#include "checker.h"
#include "include/configuration.h"

namespace sqlcheck {

Configuration state;

}  // namespace sqlcheck

int main(int argc, char **argv) {

  try {

    // Parse the input arguments from the user
    // This customizes the checker configuration
    sqlcheck::ParseArguments(
        argc, argv, sqlcheck::state);

    // Invoke the checker
    sqlcheck::Check(sqlcheck::state);

  }
  // Catching at the top level ensures that
  // destructors are always called
  catch (std::exception& exc) {
    std::cerr << exc.what() << std::endl;
    exit(EXIT_FAILURE);
  }

  return (EXIT_SUCCESS);
}
