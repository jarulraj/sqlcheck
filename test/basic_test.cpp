// BASIC TEST

#include <sstream>

#include "checker.h"

#include <gtest/gtest.h>

namespace sqlcheck {

TEST(BasicTest, RunChecker) {

  Configuration default_conf;
  default_conf.testing_mode = true;

  std::unique_ptr<std::istringstream> stream(new std::istringstream());
  stream->str(
      "SELECT    *   FROM FOO;\n"
      "\n"

      "SELECT *\n"
      "FROM BAR;\n"

      "SELECT product_id FROM Products WHERE account_id REGEXP '[[:<:]]12[[:>:]]';\n"
      );

  default_conf.test_stream.reset(stream.release());

  Check(default_conf);

}

TEST(BasicTest, MultiValuedAttributeTest) {

  Configuration default_conf;
  default_conf.testing_mode = true;

  std::unique_ptr<std::istringstream> stream(new std::istringstream());
  stream->str(
      "CREATE TABLE Products ("
      "product_id   SERIAL PRIMARY KEY,"
      "product_name VARCHAR(1000),"
      "account_id   VARCHAR(100));\n"

      "CREATE TABLE Comments ("
      "comment_id   SERIAL PRIMARY KEY,"
      "parent_id    BIGINT UNSIGNED,"
      "FOREIGN KEY (parent_id) REFERENCES Comments(comment_id));"
  );

  default_conf.test_stream.reset(stream.release());

  Check(default_conf);

}

}  // End machine sqlcheck
