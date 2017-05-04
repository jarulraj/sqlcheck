// LIST SOURCE

#include <regex>

#include "include/list.h"
#include "include/checker.h"

namespace sqlcheck {

std::string GetTableName(const std::string& sql_statement){
  std::string table_template = "create table";
  std::size_t found = sql_statement.find(table_template);
  if (found == std::string::npos) {
    return "";
  }

  // Locate table name
  auto rest = sql_statement.substr(found + table_template.size());
  // Strip space at beginning
  rest = std::regex_replace(rest, std::regex("^ +| +$|( ) +"), "$1");
  auto table_name = rest.substr(0, rest.find(' '));

  std::cout << "table_name: " << table_name;

  return table_name;
}

void CheckSelectStar(const Configuration& state,
                     const std::string& sql_statement){

  std::regex pattern("(select\\s+\\*)");
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

  std::regex pattern("(id\\s+varchar)|(id\\s+text)|(id\\s+regexp)");
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

void CheckRecursiveDependency(const Configuration& state,
                              const std::string& sql_statement){

  std::string table_name = GetTableName(sql_statement);
  if(table_name.empty()){
    return;
  }

  std::regex pattern("(references\\s+" + table_name+ ")");
  std::string title = "Recursive Dependency";
  PatternType pattern_type = PatternType::PATTERN_TYPE_CREATION;

  auto message =
      "● Avoid recursive relationships:\n"
      "It’s common for data to have recursive relationships. Data may be organized in a\n"
      "treelike or hierarchical way. However, creating a foreign key constraint to enforce\n"
      "the relationship between two columns in the same table lends to awkward querying.\n"
      "Each level of the tree corresponds to another join. You will need to issue recursive\n"
      "queries to get all descendants or all ancestors of a node.\n"
      "A solution is to construct an additional closure table. It involves storing all paths\n"
      "through the tree, not just those with a direct parent-child relationship.\n"
      "You might want to compare different hierarchical data designs -- closure table,\n"
      "path enumeration, nested sets -- and pick one based on your application's needs.\n";

  CheckPattern(state,
               sql_statement,
               pattern,
               LOG_LEVEL_ERROR,
               pattern_type,
               title,
               message);

}


}  // namespace machine

