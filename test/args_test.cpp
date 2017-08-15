// ARGS TEST

#include <sstream>

#include "checker.h"

#include <gtest/gtest.h>

namespace sqlcheck {

TEST(ArgsTest, ValidateArgs) {

  char *argv[] = {"sqlcheck", "-r 3", "-c", "-v", "-f foo.txt", NULL};
  int argc = sizeof(argv) / sizeof(char*) - 1;

  Configuration state;
  sqlcheck::ParseArguments(argc, argv, state);

}

TEST(ArgsTest, ValidateArgs2) {

  char *argv[] = {"sqlcheck", "-r 3", NULL};
  int argc = sizeof(argv) / sizeof(char*) - 1;

  Configuration state;
  sqlcheck::ParseArguments(argc, argv, state);

}

TEST(ArgsTest, HelpTest) {

  char *argv[] = {"sqlcheck", "-h", NULL};
  int argc = sizeof(argv) / sizeof(char*) - 1;

  Configuration state;
  sqlcheck::ParseArguments(argc, argv, state);

}

TEST(ArgsTest, HelpTest2) {

  char *argv[] = {"sqlcheck", "-x", NULL};
  int argc = sizeof(argv) / sizeof(char*) - 1;

  Configuration state;
  sqlcheck::ParseArguments(argc, argv, state);

}

}  // End machine sqlcheck
