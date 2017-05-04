// LIST SOURCE

#include <regex>

#include "include/list.h"
#include "include/checker.h"

namespace sqlcheck {

void CheckSelectStar(const Configuration& state,
                     const std::string& sql_statement){

  std::regex pattern("(SELECT\\s+\\*)");
  std::string title = "SELECT *";
  PatternType pattern_type = PatternType::PATTERN_TYPE_QUERY;

  std::string message1 =
      "● Inefficiency in moving data to the consumer:\n"
      "When you SELECT *, you're often retrieving more columns from the database than\n"
      "your application really needs to function. This causes more data to move from\n"
      "the database server to the client, slowing access and increasing load on your\n"
      "machines, as well as taking more time to travel across the network. This is\n"
      "especially true when someone adds new columns to underlying tables that didn't\n"
      "exist and weren't needed when the original consumers coded their data access.\n";

  std::string message2 =
      "● Indexing issues:\n"
      "Consider a scenario where you want to tune a query to a high level of performance.\n"
      "If you were to use *, and it returned more columns than you actually needed,\n"
      "the server would often have to perform more expensive methods to retrieve your\n"
      "data than it otherwise might. For example, you wouldn't be able to create an index\n"
      "which simply covered the columns in your SELECT list, and even if you did\n"
      "(including all columns [shudder]), the next guy who came around and added a column\n"
      "to the underlying table would cause the optimizer to ignore your optimized covering\n"
      "index, and you'd likely find that the performance of your query would drop\n"
      "substantially for no readily apparent reason.\n";

  std::string message3 =
      "● Binding Problems:\n"
      "When you SELECT *, it's possible to retrieve two columns of the same name from two\n"
      "different tables. This can often crash your data consumer. Imagine a query that joins\n"
      "two tables, both of which contain a column called \"ID\". How would a consumer know\n"
      "which was which? SELECT * can also confuse views (at least in some versions SQL Server)\n"
      "when underlying table structures change -- the view is not rebuilt, and the data which\n"
      "comes back can be nonsense. And the worst part of it is that you can take care to name\n"
      "your columns whatever you want, but the next guy who comes along might have no way of\n"
      "knowing that he has to worry about adding a column which will collide with your\n"
      "already-developed names.\n";

  auto message = message1 + "\n" + message2 + "\n" + message3;

  CheckPattern(state,
               sql_statement,
               pattern,
               LOG_LEVEL_ERROR,
               pattern_type,
               title,
               message);

}

void CheckMultiValuedAttribute(const Configuration& state,
                               const std::string& sql_statement){

  std::regex pattern("(id\\s+VARCHAR)|(id\\s+TEXT)|(id\\s+REGEXP)");
  std::string title = "Multi-Valued Attribute";
  PatternType pattern_type = PatternType::PATTERN_TYPE_CREATION;

  auto message =
      "● Store each value in its own column and row:\n"
      "Storing a list of IDs as a VARCHAR/TEXT column can cause performance and data integrity\n"
      "problems. Querying against such a column would require using pattern-matching\n"
      "expressions. It is awkward and costly to join a comma-separated list to matching rows.\n"
      "This will make it harder to validate IDs. Think about what is the greatest number of\n"
      "entries this list must support? Instead of using a multi-valued attribute,\n"
      "consider storing it in a separate table, so that each individual value of that attribute\n"
      "occupies a separate row. Such an intersection table implements a many-to-many relationship\n"
      "between the two referenced tables. This will greatly simplify querying and validating\n"
      "the IDs.\n";

  CheckPattern(state,
               sql_statement,
               pattern,
               LOG_LEVEL_ERROR,
               pattern_type,
               title,
               message);

}



}  // namespace machine

