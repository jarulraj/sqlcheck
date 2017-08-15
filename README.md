
[![GitHub license](https://img.shields.io/badge/license-apache-blue.svg?style=flat)](https://www.apache.org/licenses/LICENSE-2.0)
[![Build Status](https://travis-ci.org/jarulraj/sqlcheck.svg?branch=master)](https://travis-ci.org/jarulraj/sqlcheck)
[![Coverage Status](https://coveralls.io/repos/github/jarulraj/sqlcheck/badge.svg?branch=master)](https://coveralls.io/github/jarulraj/sqlcheck?branch=master)

## What Is SQLCheck?

SQLCheck automates the detection of common anti-patterns in SQL queries. Such anti-patterns often increase the time taken to run the queries on a database management system. Addressing these anti-patterns will, therefore, help improve the runtime performance of the queries. This tool targets all SQL dialects.   

## Installation

SQLCheck requires the following tools:

- **g++ 4.9+** 
- **cmake** (`apt-get install cmake`) 

Run the following commands to install SQLCheck:

```
./bootstrap
cd build
cmake -DCMAKE_BUILD_TYPE=RELEASE ..
make
```

## What it can do

Right now SQLCheck can find:

  * [[ssrf] Server Side Request Forgery](https://github.com/yandex/gixy/blob/master/docs/en/plugins/ssrf.md)

# Usage

**$ ./bin/sqlcheck sample.sql**

-------------------------------------------------
SQL Statement: **select * from foo;**  

● **Inefficiency in moving data to the consumer:**  
When you SELECT *, you're often retrieving more columns from the database than
your application really needs to function. This causes more data to move from
the database server to the client, slowing access and increasing load on your
machines, as well as taking more time to travel across the network. This is
especially true when someone adds new columns to underlying tables that didn't
exist and weren't needed when the original consumers coded their data access.

● **Indexing issues:**  
Consider a scenario where you want to tune a query to a high level of performance.
If you were to use *, and it returned more columns than you actually needed,
the server would often have to perform more expensive methods to retrieve your
data than it otherwise might. For example, you wouldn't be able to create an index
which simply covered the columns in your SELECT list, and even if you did
(including all columns [shudder]), the next guy who came around and added a column
to the underlying table would cause the optimizer to ignore your optimized covering
index, and you'd likely find that the performance of your query would drop
substantially for no readily apparent reason.

● **Binding Problems:**  
When you SELECT *, it's possible to retrieve two columns of the same name from two
different tables. This can often crash your data consumer. Imagine a query that joins
two tables, both of which contain a column called "ID". How would a consumer know
which was which? SELECT * can also confuse views (at least in some versions SQL Server)
when underlying table structures change -- the view is not rebuilt, and the data which
comes back can be nonsense. And the worst part of it is that you can take care to name
your columns whatever you want, but the next guy who comes along might have no way of
knowing that he has to worry about adding a column which will collide with your
already-developed names.

[Matching Expression: select *]

-------------------------------------------------

## References

(1) SQL Anti-patterns: Avoiding the Pitfalls of Database Programming, Bill Karwin  
(2) [Common SQL Anti-patterns](https://stackoverflow.com/questions/346659/what-are-the-most-common-sql-anti-patterns), StackOverflow

## Contributions

Contributions to SQLCheck are always welcome. You can contribute in different ways:
* Open an issue with suggestions for improvements and errors you're facing;
* Fork this repository and submit a pull request;
* Improve the documentation.

## License

Copyright (c) 2017 [CMU Database Group](http://db.cs.cmu.edu/)  
Licensed under the [Apache License](LICENSE).
