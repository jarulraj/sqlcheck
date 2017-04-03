// MAIN SOURCE

#include <iostream>
#include <fstream>

#include "configuration.h"
#include "checker.h"

namespace sqlcheck {

configuration state;

}  // namespace sqlcheck

int main(int argc, char **argv) {

  sqlcheck::ParseArguments(
      argc, argv, sqlcheck::state);

  sqlcheck::Check(sqlcheck::state);

  return 0;
}
