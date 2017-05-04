// BASIC TEST

#include <sstream>

#include "checker.h"

#include <gtest/gtest.h>

namespace sqlcheck {

TEST(BasicTest, SelectStarTest) {

  Configuration default_conf;
  default_conf.testing_mode = true;

  std::unique_ptr<std::istringstream> stream(new std::istringstream());
  stream->str(
      "SELECT    *   FROM FOO;\n"
      "\n"

      "SELECT *\n"
      "FROM BAR;\n"
      );

  default_conf.test_stream.reset(stream.release());

  Check(default_conf);

}

TEST(BasicTest, MultiValuedAttributeTest) {

  Configuration default_conf;
  default_conf.testing_mode = true;

  std::unique_ptr<std::istringstream> stream(new std::istringstream());
  stream->str(
      "SELECT product_id FROM Products WHERE account_id REGEXP '[[:<:]]12[[:>:]]';\n"

      "CREATE TABLE Products ("
      "product_id   SERIAL PRIMARY KEY,"
      "product_name VARCHAR(1000),"
      "account_id   VARCHAR(100));\n"
      );

  default_conf.test_stream.reset(stream.release());

  Check(default_conf);

}

TEST(BasicTest, RecursiveDependencyTest) {

  Configuration default_conf;
  default_conf.testing_mode = true;

  std::unique_ptr<std::istringstream> stream(new std::istringstream());
  stream->str(

      "CREATE TABLE Comments ("
      "comment_id   SERIAL PRIMARY KEY,"
      "parent_id    BIGINT UNSIGNED,"
      "FOREIGN KEY (parent_id) REFERENCES Comments(comment_id));\n"

  );

  default_conf.test_stream.reset(stream.release());

  Check(default_conf);

}

TEST(BasicTest, PrimaryKeyExistsTest) {

  Configuration default_conf;
  default_conf.testing_mode = true;

  std::unique_ptr<std::istringstream> stream(new std::istringstream());
  stream->str(

      "CREATE TABLE ArticleTags ("
      "article_id  BIGINT UNSIGNED NOT NULL,"
      "tag_id      BIGINT UNSIGNED NOT NULL,);\n"

      "CREATE TABLE BugsProducts ("
      "id SERIAL PRIMARY KEY,"
      "bug_id      BIGINT UNSIGNED NOT NULL,"
      "product_id  BIGINT UNSIGNED NOT NULL,"
      "UNIQUE KEY (bug_id, product_id),"
      "FOREIGN KEY (bug_id) REFERENCES Bugs(bug_id),"
      "FOREIGN KEY (product_id) REFERENCES Products(product_id));\n"

      "CREATE TABLE BugsProducts ("
      "bug_id      BIGINT UNSIGNED NOT NULL,"
      "id SERIAL PRIMARY KEY,"
      "product_id  BIGINT UNSIGNED NOT NULL,"
      "UNIQUE KEY (bug_id, product_id),"
      "FOREIGN KEY (bug_id) REFERENCES Bugs(bug_id),"
      "FOREIGN KEY (product_id) REFERENCES Products(product_id));\n"
  );

  default_conf.test_stream.reset(stream.release());

  Check(default_conf);

}

TEST(BasicTest, GenericPrimaryKeyTest) {

  Configuration default_conf;
  default_conf.testing_mode = true;

  std::unique_ptr<std::istringstream> stream(new std::istringstream());
  stream->str(

      "CREATE TABLE BugsProducts ("
      "id SERIAL PRIMARY KEY,"
      "bug_id      BIGINT UNSIGNED NOT NULL,"
      "product_id  BIGINT UNSIGNED NOT NULL,"
      "UNIQUE KEY (bug_id, product_id),"
      "FOREIGN KEY (bug_id) REFERENCES Bugs(bug_id),"
      "FOREIGN KEY (product_id) REFERENCES Products(product_id));\n"

      "CREATE TABLE BugsProducts ("
      "bug_id      BIGINT UNSIGNED NOT NULL,"
      "id SERIAL PRIMARY KEY,"
      "product_id  BIGINT UNSIGNED NOT NULL,"
      "UNIQUE KEY (bug_id, product_id),"
      "FOREIGN KEY (bug_id) REFERENCES Bugs(bug_id),"
      "FOREIGN KEY (product_id) REFERENCES Products(product_id));\n"
  );

  default_conf.test_stream.reset(stream.release());

  Check(default_conf);

}

TEST(BasicTest, ForeignKeyExistsTest) {

  Configuration default_conf;
  default_conf.testing_mode = true;

  std::unique_ptr<std::istringstream> stream(new std::istringstream());
  stream->str(

      "CREATE TABLE BugsProducts ("
      "id SERIAL PRIMARY KEY,"
      "bug_id      BIGINT UNSIGNED NOT NULL,"
      "product_id  BIGINT UNSIGNED NOT NULL,"
      "UNIQUE KEY (bug_id, product_id));\n"

  );

  default_conf.test_stream.reset(stream.release());

  Check(default_conf);

}

}  // End machine sqlcheck
