// MAIN SOURCE

#include <iostream>
#include <fstream>

#include "checker.h"
#include "include/configuration.h"

namespace sqlcheck {

Configuration state;

}  // namespace sqlcheck

int main(int argc, char **argv) {

  sqlcheck::ParseArguments(
      argc, argv, sqlcheck::state);

  sqlcheck::Check(sqlcheck::state);

  return 0;
}
