// MAIN SOURCE

#include <iostream>
#include <fstream>

#include "configuration.h"
#include "checker.h"

#include <glog/logging.h>

namespace sqlcheck {

configuration state;

}  // namespace sqlcheck

int main(int argc, char **argv) {

  // Initialize Google's logging library.
  google::InitGoogleLogging(argv[0]);

  sqlcheck::ParseArguments(
      argc, argv, sqlcheck::state);

  sqlcheck::Check();

  return 0;
}
