// ARGS TEST

#include <sstream>

#include "checker.h"

#include <gtest/gtest.h>

namespace sqlcheck {

TEST(ArgsTest, ValidateArgs) {

  char *argv[] = {"sqlcheck", "-l 3", "-f foo.txt", NULL};
  int argc = sizeof(argv) / sizeof(char*) - 1;

  Configuration state;
  sqlcheck::ParseArguments(argc, argv, state);

}

TEST(ArgsTest, HelpTest) {

  char *argv[] = {"sqlcheck", "-x", NULL};
  int argc = sizeof(argv) / sizeof(char*) - 1;

  Configuration state;
  sqlcheck::ParseArguments(argc, argv, state);

}

}  // End machine sqlcheck
