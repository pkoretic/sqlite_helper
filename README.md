sqlite_helper
=============

**SQLite C++ wrapper library for easier database management**    
note: sqlite library is obviously needed



**Compile**

    gcc -c sqlite_helper.cpp -lsqlite3 -ldl -lpthread  (optional -O3 -std=gnu++0x -Wall -g)

**Open database**

    database db("sqlite.db");        //open database read only
    database db("sqlite.db, false"); //open database read only
    database db("sqlite.db, true");  //open database read/write

**Close database**

    db.close();                      // database is automatically closed when destructor is called

    
**EXECute query** - ignore returned data (insert/delete)

    string status = db.exec("delete row from table where id='1'");
    if(status) 
      cout << "Deleted" << endl;
    else
      cout << "Delete failed" << db.error << endl;
    
**Query single** - return single row or entry from database

    // return single entry
    cout << db.query("select user from table where id='1') << endl;
     
    // return complete row
     vector<string> row = db.query("select name, surname from table where id='1', true/false);
     
     cout << row.at(0) << row.at(1) << endl;

**Query** - execute normal query

    vector<vector<string>> data = db->querySingle("select name, surname from table");
    cout << data.at(0).at(0) << data.at(0).at(1) << endl;  // name1 surname1
    cout << data.at(1).at(0) << data.at(1).at(1) << endl;  // name2 surname2

    
**Error checking**

    After every query one can always check if db.error.size() != 0 to see if there was some error.
