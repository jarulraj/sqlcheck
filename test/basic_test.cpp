// BASIC TEST

#include <sstream>

#include "checker.h"

#include <gtest/gtest.h>

namespace sqlcheck {

TEST(BasicTest, SelectStarTest) {

  Configuration default_conf;
  default_conf.testing_mode = true;
  default_conf.verbose_mode = true;

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
      "bugs_fixed_2010  INT,"
      "2010  INT"
      ");\n"

      "CREATE TABLE Bugs_2008 ( . . . );\n"

      "ALTER TABLE Customers ADD (revenue2002 NUMBER(9,2));\n"

      "CREATE TABLE Bugs_2009 (-- other columns"
      "date_reported DATE CHECK (EXTRACT(YEAR FROM date_reported) = 2009));\n"

  );

  default_conf.test_stream.reset(stream.release());

  Check(default_conf);

}

TEST(BasicTest, FloatTest) {

  Configuration default_conf;
  default_conf.testing_mode = true;

  std::unique_ptr<std::istringstream> stream(new std::istringstream());
  stream->str(

      "ALTER TABLE Bugs ADD COLUMN hours FLOAT;\n"

      "SELECT * FROM Accounts WHERE ABS(hourly_rate - 59.95) < 0.0000001;\n"

      "ALTER TABLE Bugs ADD COLUMN hours NUMERIC(9,2);\n"

  );

  default_conf.test_stream.reset(stream.release());

  Check(default_conf);

}

void PrintMatches(const std::string& in, const std::string& re){

  std::smatch m;
  std::regex_search(in, m, std::regex(re));

  if(m.empty()) {
    std::cout << "input=[" << in << "], regex=[" << re << "]: NO MATCH\n";
  } else {
    std::cout << "input=[" << in << "], regex=[" << re << "]: ";
    std::cout << "prefix=[" << m.prefix() << "] ";
    for(std::size_t n = 0; n < m.size(); ++n)
      std::cout << " m[" << n << "]=[" << m[n] << "] ";
    std::cout << "suffix=[" << m.suffix() << "]\n";
  }

}

TEST(BasicTest, RegexTests) {

  // greedy match, repeats [a-z] 4 times
  PrintMatches("abcdefghi", "a[a-z]{2,4}");

  // non-greedy match, repeats [a-z] 2 times
  PrintMatches("abcdefghi", "a[a-z]{2,4}?");

  // Choice point ordering for quantifiers results in a match
  // with two repetitions, first matching the substring "aa",
  // second matching the substring "ba", leaving "ac" not matched
  // ("ba" appears in the capture clause m[1])
  PrintMatches("aabaac", "(aa|aabaac|ba|b|c)*");

  // Choice point ordering for quantifiers makes this regex
  // calculate the greatest common divisor between 10 and 15
  // (the answer is 5, and it populates m[1] with "aaaaa")
  PrintMatches("aaaaaaaaaa,aaaaaaaaaaaaaaa", "^(a+)\\1*,\\1+$");

  // the substring "bbb" does not appear in the capture clause m[4]
  // because it is cleared when the second repetition of the atom
  // (a+)?(b+)?(c) is matching the substring "ac"
  PrintMatches("zaacbbbcac", "(z)((a+)?(b+)?(c))*");

}

TEST(BasicTest, MetadataRegexTests) {

  std::string regex = "[A-za-z\\-_@]+[0-9]+ ";

  // metadata tribbles
  PrintMatches("bugs_2004", regex);
  PrintMatches("bugs8_foo", regex);

  // metadata tribbles
  PrintMatches("bugs_2004", regex);
  PrintMatches("CREATE TABLE Bugs_2008 ( . . . )", regex);
  PrintMatches("CREATE TABLE Bugs_200843214 ( . . . )", regex);
  PrintMatches("CREATE TABLE tag8 ( . . . )", regex);

}

TEST(BasicTest, PhysicalDesignTests) {

  Configuration default_conf;
  default_conf.testing_mode = true;

  std::unique_ptr<std::istringstream> stream(new std::istringstream());
  stream->str(

      "CREATE TABLE Bugs ("
      "-- other columns"
      "status ENUM('NEW', 'IN PROGRESS', 'FIXED'), );\n"

      "CREATE TABLE Bugs ("
      "-- other columns"
      "status VARCHAR(20) CHECK (status IN ('NEW', 'IN PROGRESS', 'FIXED')) );\n"

      "ALTER TABLE Bugs MODIFY COLUMN status ENUM('NEW', 'IN PROGRESS', 'CODE COMPLETE', 'VERIFIED');\n"

      "CREATE TABLE Screenshots ("
      "bug_id            BIGINT UNSIGNED NOT NULL,"
      "image_id          BIGINT UNSIGNED NOT NULL,"
      "screenshot_path   VARCHAR(100),"
      "caption           VARCHAR(100),"
      "PRIMARY KEY       (bug_id, image_id),"
      "FOREIGN KEY (bug_id) REFERENCES Bugs(bug_id)"
      ");\n"

      "unlink('images/screenshot1234-1.jpg' );\n"

      "CREATE TABLE Bugs ("
      "bug_id SERIAL PRIMARY KEY,"
      "date_reported DATE NOT NULL,"
      "INDEX (bug_id),"
      "INDEX (summary),"
      "INDEX (hours),"
      "INDEX (bug_id, date_reported, status)"
      ");\n"

      "CREATE INDEX TelephoneBook ON Accounts(last_name, first_name);\n"

  );

  default_conf.test_stream.reset(stream.release());

  Check(default_conf);

}

TEST(BasicTest, QueryTests) {

  Configuration default_conf;
  default_conf.testing_mode = true;

  std::unique_ptr<std::istringstream> stream(new std::istringstream());
  stream->str(

      "SELECT * FROM Bugs WHERE assigned_to IS NULL OR assigned_to <> 1;\n"

      "SELECT first_name || ' ' || last_name AS full_name FROM Accounts;\n"

      "SELECT product_id, MAX(date_reported) AS latest,"
      "MIN(date_reported) AS earliest, bug_id"
      "FROM Bugs JOIN BugsProducts USING (bug_id)"
      "GROUP BY product_id;\n"

      "SELECT product_id, MAX(date_reported) AS latest"
      "FROM Bugs JOIN BugsProducts USING (bug_id)"
      "GROUP BY product_id;\n"

      "SELECT * FROM Bugs ORDER BY RAND() LIMIT 1;\n"

      "SELECT b1.*"
      "FROM Bugs AS b1"
      "JOIN (SELECT CEIL(RAND() * (SELECT MAX(bug_id) FROM Bugs)) AS rand_id) AS b2"
      "ON (b1.bug_id = b2.rand_id);\n"

      "SELECT * FROM Bugs WHERE description LIKE '%crash%';\n"

      "SELECT * FROM Bugs WHERE description REGEXP 'crash';\n"

      "SELECT COUNT(bp.product_id) AS how_many_products,"
      "COUNT(dev.account_id) AS how_many_developers,"
      "COUNT(b.bug_id)/COUNT(dev.account_id) AS avg_bugs_per_developer,"
      "COUNT(cust.account_id) AS how_many_customers"
      "FROM Bugs b JOIN BugsProducts bp ON (b.bug_id = bp.bug_id)"
      "JOIN Accounts dev ON (b.assigned_to = dev.account_id)"
      "JOIN Accounts cust ON (b.reported_by = cust.account_id) WHERE cust.email NOT LIKE '%@example.com'"
      "JOIN Accounts cust ON (b.reported_by = cust.account_id) WHERE cust.email NOT LIKE '%@example.com'"
      "JOIN Accounts cust ON (b.reported_by = cust.account_id) WHERE cust.email NOT LIKE '%@example.com'"
      "JOIN Accounts cust ON (b.reported_by = cust.account_id) WHERE cust.email NOT LIKE '%@example.com'"
      "JOIN Accounts cust ON (b.reported_by = cust.account_id) WHERE cust.email NOT LIKE '%@example.com'"
      "GROUP BY bp.product_id;"

      "SELECT AVG(bugs_per_developer) AS average_bugs_per_developer"
      "FROM (SELECT dev.account_id, COUNT(*) AS bugs_per_developer"
      "FROM Bugs b JOIN Accounts dev"
      "ON (b.assigned_to = dev.account_id)"
      "WHERE b.status = 'FIXED' GROUP BY dev.account_id) t;\n"

      "INSERT INTO Bugs VALUES (DEFAULT, CURDATE(), 'New bug', 'Test T987 fails...',"
      "NULL, 123, NULL, NULL, DEFAULT, 'Medium', NULL);\n"

      "INSERT INTO Accounts (account_name, first_name, last_name, email,"
      "password_hash, portrait_image, hourly_rate)"
      "VALUES ('bkarwin', 'Bill', 'Karwin', 'bill@example.com', SHA2('xyzzy'), NULL, 49.95);\n"

      "CREATE TABLE Accounts ("
      "account_id    SERIAL PRIMARY KEY,"
      "account_name  VARCHAR(20) NOT NULL,"
      "email         VARCHAR(100) NOT NULL,"
      "password      VARCHAR(30) NOT NULL"
      ");\n"

      "SELECT * FROM Accounts"
      "WHERE account_name = 'bill' AND password = 'opensesame';\n"

      "SELECT s.cust_id,count(s.cust_id) FROM  SH.sales s GROUP BY s.cust_id\n"
      "HAVING s.cust_id != '1660' AND s.cust_id != '2';\n"

      "SELECT * FROM SH.products p WHERE p.prod_id = (SELECT s.prod_id FROM SH.sales s"
      "WHERE s.cust_id = 100996 AND s.quantity_sold = 1 );\n"

      "SELECT s.* FROM SH.sales s WHERE s.prod_id = 14 OR s.prod_id = 17;\n"

      "SELECT cust_id FROM SH.sales UNION SELECT cust_id FROM customers;\n"

      "SELECT DISTINCT c.country_id, c.country_name FROM SH.countries c JOIN SH.customers e"
      "WHERE e.country_id = c.country_id;\n"

  );

  default_conf.test_stream.reset(stream.release());

  Check(default_conf);

}


}  // End machine sqlcheck
