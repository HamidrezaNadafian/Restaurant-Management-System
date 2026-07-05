#ifndef OPENDATABASE_H
#define OPENDATABASE_H

#include "lib.h"
#include "sqlite3.h"

class DataBase
{
    private:
        sqlite3 *db;
        
    public:
        DataBase()
        {
            int exit = sqlite3_open("Maindb.db", &db);
            if (exit){
                cerr << "Error open DB: " << sqlite3_errmsg(db) << endl;
            } 
        }

        sqlite3* getdb(){
            return db;
        }

};

#endif