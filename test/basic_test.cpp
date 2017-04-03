// BASIC TEST

#include <sstream>

#include "checker.h"

#include <gtest/gtest.h>

namespace sqlcheck {

TEST(BasicTest, RunChecker) {

  configuration default_conf;
  default_conf.testing_mode = true;

  std::unique_ptr<std::istringstream> stream(new std::istringstream());
  stream->str(
      "SELECT *\n"
      "FROM FOO;\n"
      "\n"
      "SELECT *\n"
      "FROM BAR;\n"
      );

  default_conf.test_stream.reset(stream.release());

  Check(default_conf);

}

}  // End machine sqlcheck
