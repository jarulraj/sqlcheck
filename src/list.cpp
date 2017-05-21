// LIST SOURCE

#include <regex>

#include "include/list.h"
#include "include/checker.h"

namespace sqlcheck {

// UTILITY

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

  return table_name;
}

bool IsCreateStatement(const std::string& sql_statement){
  std::string table_template = "create table";
  std::size_t found = sql_statement.find(table_template);
  if (found == std::string::npos) {
    return false;
  }

  return true;
}

// LOGICAL DATABASE DESIGN


void CheckMultiValuedAttribute(const Configuration& state,
                               const std::string& sql_statement,
                               bool& print_statement){

  std::regex pattern("(id\\s+varchar)|(id\\s+text)|(id\\s+regexp)");
  std::string title = "Multi-Valued Attribute";
  PatternType pattern_type = PatternType::PATTERN_TYPE_LOGICAL_DATABASE_DESIGN;

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
               print_statement,
               pattern,
               LOG_LEVEL_ERROR,
               pattern_type,
               title,
               message,
               true);

}

void CheckRecursiveDependency(const Configuration& state,
                              const std::string& sql_statement,
                              bool& print_statement){

  std::string table_name = GetTableName(sql_statement);
  if(table_name.empty()){
    return;
  }

  std::regex pattern("(references\\s+" + table_name+ ")");
  std::string title = "Recursive Dependency";
  PatternType pattern_type = PatternType::PATTERN_TYPE_LOGICAL_DATABASE_DESIGN;

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
               print_statement,
               pattern,
               LOG_LEVEL_ERROR,
               pattern_type,
               title,
               message,
               true);

}

void CheckPrimaryKeyExists(const Configuration& state,
                           const std::string& sql_statement,
                           bool& print_statement){

  auto create_statement = IsCreateStatement(sql_statement);
  if(create_statement == false){
    return;
  }

  std::regex pattern("(primary key)");
  std::string title = "Primary Key Exists";
  PatternType pattern_type = PatternType::PATTERN_TYPE_LOGICAL_DATABASE_DESIGN;

  auto message =
      "● Consider adding a primary key:\n"
      "A primary key constraint is important when you need to do the following:\n"
      "prevent a table from containing duplicate rows,\n"
      "reference individual rows in queries, and\n"
      "support foreign key references\n"
      "If you don’t use primary key constraints, you create a chore for yourself:\n"
      "checking for duplicate rows. More often than not, you will need to define\n"
      "a primary key for every table. Use compound keys when they are appropriate.\n";

  CheckPattern(state,
               sql_statement,
               print_statement,
               pattern,
               LOG_LEVEL_WARN,
               pattern_type,
               title,
               message,
               false);

}

void CheckGenericPrimaryKey(const Configuration& state,
                            const std::string& sql_statement,
                            bool& print_statement){

  auto create_statement = IsCreateStatement(sql_statement);
  if(create_statement == false){
    return;
  }

  std::regex pattern("(\\s+[\\(]?id\\s+)|(,id\\s+)|(\\s+id\\s+serial)");
  std::string title = "Generic Primary Key";
  PatternType pattern_type = PatternType::PATTERN_TYPE_LOGICAL_DATABASE_DESIGN;

  auto message =
      "● Skip using a generic primary key (id):\n"
      "Adding an id column to every table causes several effects that make its\n"
      "use seem arbitrary. You might end up creating a redundant key or allow\n"
      "duplicate rows if you add this column in a compound key.\n"
      "The name id is so generic that it holds no meaning. This is especially\n"
      "important when you join two tables and they have the same primary\n"
      "key column name.\n";

  CheckPattern(state,
               sql_statement,
               print_statement,
               pattern,
               LOG_LEVEL_ERROR,
               pattern_type,
               title,
               message,
               true);

}

void CheckForeignKeyExists(const Configuration& state,
                           const std::string& sql_statement,
                           bool& print_statement){

  auto create_statement = IsCreateStatement(sql_statement);
  if(create_statement == false){
    return;
  }

  std::regex pattern("(foreign key)");
  std::string title = "Foreign Key Exists";
  PatternType pattern_type = PatternType::PATTERN_TYPE_LOGICAL_DATABASE_DESIGN;

  auto message =
      "● Consider adding a foreign key:\n"
      "Are you leaving out the application constraints? Even though it seems at\n"
      "first that skipping foreign key constraints makes your database design\n"
      "simpler, more flexible, or speedier, you pay for this in other ways.\n"
      "It becomes your responsibility to write code to ensure referential integrity\n"
      "manually. Use foreign key constraints to enforce referential integrity.\n"
      "Foreign keys have another feature you can’t mimic using application code:\n"
      "cascading updates to multiple tables. This feature allows you to\n"
      "update or delete the parent row and lets the database takes care of any child\n"
      "rows that reference it. The way you declare the ON UPDATE or ON DELETE clauses\n"
      "in the foreign key constraint allow you to control the result of a cascading\n"
      "operation. Make your database mistake-proof with constraints.\n";

  CheckPattern(state,
               sql_statement,
               print_statement,
               pattern,
               LOG_LEVEL_WARN,
               pattern_type,
               title,
               message,
               false);

}

void CheckVariableAttribute(const Configuration& state,
                            const std::string& sql_statement,
                            bool& print_statement){

  std::string table_name = GetTableName(sql_statement);
  if(table_name.empty()){
    return;
  }

  auto found = table_name.find("attribute");
  if (found == std::string::npos) {
    return;
  }

  std::regex pattern("(attribute)");
  std::string title = "Entity-Attribute-Value Pattern";
  PatternType pattern_type = PatternType::PATTERN_TYPE_LOGICAL_DATABASE_DESIGN;

  auto message =
      "● Dynamic schema with variable attributes:\n"
      "Are you trying to create a schema where you can define new attributes\n"
      "at runtime.? This involves storing attributes as rows in an attribute table.\n"
      "This is referred to as the Entity-Attribute-Value or schemaless pattern.\n"
      "When you use this pattern,  you sacrifice many advantages that a conventional\n"
      "database design would have given you. You can't make mandatory attributes.\n"
      "You can't enforce referential integrity. You might find that attributes are\n"
      "not being named consistently. A solution is to store all related types in one table,\n"
      "with distinct columns for every attribute that exists in any type\n"
      "(Single Table Inheritance). Use one attribute to define the subtype of a given row.\n"
      "Many attributes are subtype-specific, and these columns must\n"
      "be given a null value on any row storing an object for which the attribute\n"
      "does not apply; the columns with non-null values become sparse.\n"
      "Another solution is to create a separate table for each subtype\n"
      "(Concrete Table Inheritance). A third solution mimics inheritance,\n"
      "as though tables were object-oriented classes (Class Table Inheritance).\n"
      "Create a single table for the base type, containing attributes common to\n"
      "all subtypes. Then for each subtype, create another table, with a primary key\n"
      "that also serves as a foreign key to the base table.\n"
      "If you have many subtypes or if you must support new attributes frequently,\n"
      "you can add a BLOB column to store data in a format such as XML or JSON,\n"
      "which encodes both the attribute names and their values.\n"
      "This design is best when you can’t limit yourself to a finite set of subtypes\n"
      "and when you need complete flexibility to define new attributes at any time.\n";

  CheckPattern(state,
               sql_statement,
               print_statement,
               pattern,
               LOG_LEVEL_WARN,
               pattern_type,
               title,
               message,
               true);

}

void CheckMultiColumnAttribute(const Configuration& state,
                               const std::string& sql_statement,
                               bool& print_statement){

  auto create_statement = IsCreateStatement(sql_statement);
  if(create_statement == false){
    return;
  }

  std::regex pattern("([A-za-z][\\-_@]?[0-9]+\%)");
  std::string title = "Multi-Column Attribute";
  PatternType pattern_type = PatternType::PATTERN_TYPE_LOGICAL_DATABASE_DESIGN;

  auto message =
      "● Store each value with the same meaning in a single column:\n"
      "Creating multiple columns in a table indicates that you are trying to store\n"
      "a multivalued attribute. This design makes it hard to add or remove values,\n"
      "to ensure the uniqueness of values, and handling growing sets of values.\n"
      "The best solution is to create a dependent table with one column for the\n"
      "multivalue attribute. Store the multiple values in multiple rows instead of\n"
      "multiple columns. Also, define a foreign key in the dependent table to associate\n"
      "the values to its parent row.\n";

  CheckPattern(state,
               sql_statement,
               print_statement,
               pattern,
               LOG_LEVEL_ERROR,
               pattern_type,
               title,
               message,
               true);

}

void CheckMetadataTribbles(const Configuration& state,
                           const std::string& sql_statement,
                           bool& print_statement){

  auto create_statement = IsCreateStatement(sql_statement);
  if(create_statement == false){
    return;
  }

  std::regex pattern("^[A-za-z][\\-_@]?[0-9]{4}$");
  std::string title = "Metadata Tribbles";
  PatternType pattern_type = PatternType::PATTERN_TYPE_LOGICAL_DATABASE_DESIGN;

  auto message =
      "● Breaking down a table or column by year:\n"
      "You might be trying to split a single column into multiple columns,\n"
      "using column names based on distinct values in another attribute.\n"
      "Each year, you will need to add one more column or table.\n"
      "You are mixing metadata with data. You will now need to make sure that\n"
      "the primary key values are unique across all the split columns or tables.\n"
      "The solution is to use a feature called sharding or horizontal partitioning.\n"
      "(PARTITION BY HASH ( YEAR(...) ). With this feature, you can gain the\n"
      "benefits of splitting a large table without the drawbacks.\n"
      "Partitioning is not defined in the SQL standard, so each brand of database\n"
      "implements it in their own nonstandard way.\n"
      "Another remedy for metadata tribbles is to create a dependent table.\n"
      "Instead of one row per entity with multiple columns for each year,\n"
      "use multiple rows. Don't let data spawn metadata.\n";

  CheckPattern(state,
               sql_statement,
               print_statement,
               pattern,
               LOG_LEVEL_ERROR,
               pattern_type,
               title,
               message,
               true);

}

// PHYSICAL DATABASE DESIGN

void CheckFloat(const Configuration& state,
                const std::string& sql_statement,
                bool& print_statement){

  std::regex pattern("(float)|(real)|(double precision)|(0\\.000[0-9]*)");
  std::string title = "Imprecise Data Type";
  PatternType pattern_type = PatternType::PATTERN_TYPE_LOGICAL_DATABASE_DESIGN;

  auto message =
      "● Use precise data types:\n"
      "Virtually any use of FLOAT, REAL, or DOUBLE PRECISION data types is suspect.\n"
      "Most applications that use floating-point numbers don't require the range of\n"
      "values supported by IEEE 754 formats. The cumulative impact of inexact \n"
      "floating-point numbers is severe when calculating aggregates.\n"
      "Instead of FLOAT or its siblings, use the NUMERIC or DECIMAL SQL data types\n"
      "for fixed-precision fractional numbers. These data types store numeric values\n"
      "exactly, up to the precision you specify in the column definition.\n"
      "Do not use FLOAT if you can avoid it.\n";

  CheckPattern(state,
               sql_statement,
               print_statement,
               pattern,
               LOG_LEVEL_ERROR,
               pattern_type,
               title,
               message,
               true);

}


// QUERY

void CheckSelectStar(const Configuration& state,
                     const std::string& sql_statement,
                     bool& print_statement){

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
               print_statement,
               pattern,
               LOG_LEVEL_ERROR,
               pattern_type,
               title,
               message,
               true);

}


}  // namespace machine

