// LIST SOURCE

#include "include/list.h"
#include "include/checker.h"

namespace sqlcheck {

void CheckSelectStar(const Configuration& state,
                     const std::string& sql_statement){

  std::string pattern = "SELECT *";
  std::string title = "SELECT *";

  std::string message1 =
      "1) Inefficiency in moving data to the consumer:\n"
      "When you SELECT *, you're often retrieving more columns from the database than\n"
      "your application really needs to function. This causes more data to move from\n"
      "the database server to the client, slowing access and increasing load on your\n"
      "machines, as well as taking more time to travel across the network. This is\n"
      "especially true when someone adds new columns to underlying tables that didn't\n"
      "exist and weren't needed when the original consumers coded their data access.\n\n";

  std::string message2 =
      "2) Indexing issues:\n"
      "Consider a scenario where you want to tune a query to a high level of performance.\n"
      "If you were to use *, and it returned more columns than you actually needed,\n"
      "the server would often have to perform more expensive methods to retrieve your \n"
      "data than it otherwise might. For example, you wouldn't be able to create an index\n"
      "which simply covered the columns in your SELECT list, and even if you did \n"
      "(including all columns [shudder]), the next guy who came around and added a column\n"
      "to the underlying table would cause the optimizer to ignore your optimized covering\n"
      "index, and you'd likely find that the performance of your query would drop \n"
      "substantially for no readily apparent reason.\n\n";

  std::string message3 =
      "3) Binding Problems:\n"
      "When you SELECT *, it's possible to retrieve two columns of the same name from two\n"
      "different tables. This can often crash your data consumer. Imagine a query that joins\n"
      "two tables, both of which contain a column called \"ID\". How would a consumer know\n"
      "which was which? SELECT * can also confuse views (at least in some versions SQL Server)\n"
      "when underlying table structures change -- the view is not rebuilt, and the data which\n"
      "comes back can be nonsense. And the worst part of it is that you can take care to name\n"
      "your columns whatever you want, but the next guy who comes along might have no way of\n"
      "knowing that he has to worry about adding a column which will collide with your\n"
      "already-developed names.\n\n";

  CheckPattern(state,
               sql_statement,
               pattern,
               LOG_LEVEL_ERROR,
               title,
               message1 + message2 + message3);

}


}  // namespace machine

