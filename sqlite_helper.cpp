/*======================================================================
* SQLITE HELPER
* Petar Koretic
* READ sqlite_helper.hpp for info
* =====================================================================*/

#include <string>
#include <vector>
#include <sqlite3.h>
#include "sqlite_helper.hpp"


database::database(const char* filename)
{
  sqlite3_open_v2(filename, &db, SQLITE_OPEN_SHAREDCACHE | SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READONLY, NULL);
}
database::database(const char* filename, bool writeMode)
{
  // open with multiple thread support and for maximum performance
  sqlite3_open_v2(filename, &db, SQLITE_OPEN_SHAREDCACHE | SQLITE_OPEN_FULLMUTEX | (writeMode ? SQLITE_OPEN_READWRITE : SQLITE_OPEN_READONLY), NULL );
  char *errMsg=0;
  sqlite3_exec(db, "PRAGMA synchronous = OFF", 0, 0, &errMsg);
  //sqlite3_exec(db, "PRAGMA journal_mode = WAL", 0, 0, &errMsg); // use this if you want a safer method
}

bool database::exec(const std::string &query)
{
  std::vector<std::string> result;
  char *errMsg=0;
  int rc = sqlite3_exec(db,query.c_str(),0,0,&errMsg);
  if(rc!=SQLITE_OK) { error=errMsg; return false; }
  error.clear();

  return true;
}

std::string database::querySingle(const std::string &query)
{
  std::string result;
  sqlite3_stmt *statement;

  if(sqlite3_prepare_v2(db, query.c_str(), -1, &statement, 0) == SQLITE_OK)
  {
    if(sqlite3_column_count(statement) && sqlite3_step(statement) == SQLITE_ROW)
    {
      char *ptr = (char*) sqlite3_column_text(statement, 0);
      if(ptr) result = ptr;
    }
    sqlite3_finalize(statement);
  }

  std::string error_ = sqlite3_errmsg(db);

  if(error_ != "not an error")
    error=error_;
  else
    error.clear();

  return result;
}

std::vector<std::string> database::querySingle(const std::string &query, bool row)
{
  std::vector<std::string> result;
  sqlite3_stmt *statement;

  if(sqlite3_prepare_v2(db, query.c_str(), -1, &statement, 0) == SQLITE_OK)
  {
    int cols=sqlite3_column_count(statement);

    if(cols && sqlite3_step(statement) == SQLITE_ROW)
    for(int col = 0; col < cols; col++)
    {
       std::string  val;
        char *ptr = (char*)sqlite3_column_text(statement, col);

        if(ptr) val = ptr;

        result.push_back(val);
    }

    sqlite3_finalize(statement);
  }

  std::string error_ = sqlite3_errmsg(db);

  if(error_ != "not an error")
    error=error_;
  else
    error.clear();

  return result;
}

std::vector<std::vector<std::string>> database::query(const std::string &query)
{
  sqlite3_stmt *statement;
  std::vector<std::vector<std::string>> results;

  if(sqlite3_prepare_v2(db, query.c_str(), -1, &statement, 0) == SQLITE_OK)
  {
    int cols = sqlite3_column_count(statement);

    if(cols)
    while(true)
    {
      if( sqlite3_step(statement) != SQLITE_ROW ) break;

     std::vector<std::string> values;
      for(int col = 0; col < cols; col++)
      {
       std::string  val;
        char *ptr = (char*)sqlite3_column_text(statement, col);

        if(ptr) val = ptr;

        values.push_back(val);
      }
      results.push_back(values);
    }

    sqlite3_finalize(statement);
  }

  std::string error_ = sqlite3_errmsg(db);

  if(error_ != "not an error")
    error=error_;
  else
    error.clear();

  return results;
}
