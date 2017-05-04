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

TEST(BasicTest, VariableAttributeTest) {

  Configuration default_conf;
  default_conf.testing_mode = true;

  std::unique_ptr<std::istringstream> stream(new std::istringstream());
  stream->str(

      "CREATE TABLE IssueAttributes ("
      "issue_id    BIGINT UNSIGNED NOT NULL,"
      "attr_name   VARCHAR(100) NOT NULL,"
      "attr_value  VARCHAR(100),"
      "PRIMARY KEY (issue_id, attr_name),"
      "FOREIGN KEY (issue_id) REFERENCES Issues(issue_id)"
      ");"

  );

  default_conf.test_stream.reset(stream.release());

  Check(default_conf);

}

TEST(BasicTest, MultiColumnAttributeTest) {

  Configuration default_conf;
  default_conf.testing_mode = true;

  std::unique_ptr<std::istringstream> stream(new std::istringstream());
  stream->str(

      "CREATE TABLE Bugs ("
      "bug_id      SERIAL PRIMARY KEY,"
      "description VARCHAR(1000),"
      "tag1        VARCHAR(20),"
      "tag2        VARCHAR(20),"
      "tag3        VARCHAR(20)"
      ");"

      );

  default_conf.test_stream.reset(stream.release());

  Check(default_conf);

}

TEST(BasicTest, MetadataTribblesTest) {

  Configuration default_conf;
  default_conf.testing_mode = true;

  std::unique_ptr<std::istringstream> stream(new std::istringstream());
  stream->str(

      "CREATE TABLE ProjectHistory ("
      "bugs_fixed_2008  INT,"
      "bugs_fixed_2009  INT,"
      "bugs_fixed_2010  INT"
      ");\n"

      "CREATE TABLE Bugs_2008 ( . . . );\n"

      "ALTER TABLE Customers ADD (revenue2002 NUMBER(9,2));\n"

      "CREATE TABLE Bugs_2009 (-- other columns"
      "date_reported DATE CHECK (EXTRACT(YEAR FROM date_reported) = 2009));\n"

      );

  default_conf.test_stream.reset(stream.release());

  Check(default_conf);

}

}  // End machine sqlcheck
