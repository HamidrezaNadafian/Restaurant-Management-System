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
           const char* path = "C:/Users/User/Desktop/Project/main/Maindb.db";

            int exit = sqlite3_open(path, &db);

         
        }

        sqlite3* getdb(){
            return db;
        }

};

#endif