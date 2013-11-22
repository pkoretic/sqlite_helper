/*======================================================================
* SQLITE HELPER 
* Helper functions desgined for easy management of sqlite databases
* Databe is open by constructor and closed automatically by destructor
* Primary things here are multithreading support and speed
* Petar Koretic
* =====================================================================*/

#ifndef SQLITE_HELPER_H
#define SQLITE_HELPER_H

#include <sqlite3.h>

class database
{
public:
  // if error happens we save it here
  std::string error;

  /*=========================================================================
    OPEN database
    accepts: const char* filename    - filename (with path if needed) to the database
             [ bool writeMode=false] - true = read/write - false = readonly
    example
             database db("sqlite.db");
  ==========================================================================*/
  database(const char* filename);
  database(const char* filename, bool writeMode);

  // close database when constructor gets called
  ~database() { sqlite3_close(db); }

  /*======================================================================================
   EXECute query and ignore returned data
   accepts: string - query which will get executed
   returns: bool   - true if it went ok, false if error happened [read database.error]
   example
            bool status = db->exec("delete from table");
            if(status) 
              cout << "OK" << endl;
            else 
              cout << "NOT OK:" << db.error << endl; 
   ======================================================================================*/
  bool exec(const std::string &query);

  /*======================================================================================
   Run query and return single entry from database
   accepts: string - query which will get executed
   returns: string - one value which was found
   example
            string name = db->querySingle("select name from table where ID='1'");
            cout << name << endl;
   ======================================================================================*/
  std::string querySingle(const std::string &query);

  /*======================================================================================
   Run query and return single row from database
   accepts: string - query which will get executed
   returns: vector<string> - row values which were found
   example
           vector<string> data =  db->querySingle("select name, surname from table where ID='1'", true);
           cout << data.at(0) << data.at(1) << endl;
   ======================================================================================*/
  std::vector<std::string> querySingle(const std::string &query, bool row);

  /*======================================================================================
   Run complete query and return all data from databasee
   accepts: string - query which will get executed
   returns: vector<vector<string>> - rows which were found
   example
            vector<vector<string>> data = db->querySingle("select name, surname from table");
            cout << data.at(0).at(0) << data.at(0).at(1) << endl;  // name1 surname1
            cout << data.at(1).at(0) << data.at(1).at(1) << endl;  // name2 surname2
   ======================================================================================*/
  std::vector<std::vector<std::string>> query(const std::string &query);

  // close database manually if desired
  void close() { sqlite3_close(db); } 

private:
  sqlite3 *db;
};

#endif
