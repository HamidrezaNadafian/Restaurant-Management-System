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
            const char* path = "Maindb.db";

            int exit = sqlite3_open(path, &db);

            if (exit != SQLITE_OK){
                cerr << sqlite3_errmsg(db) <<"\n";
            }


        }
        ~DataBase()
        {
            if (db) {
                sqlite3_close(db);
            }
        }

        sqlite3* getdb(){
            return db;
        }

};

#endif