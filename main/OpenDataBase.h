#ifndef OPENDATABASE_H
#define OPENDATABASE_H

#include "lib.h"
#include "sqlite3.h"

// #include <QMessageBox>
// #include <QApplication>

class DataBase
{
    private:
        sqlite3 *db;
        
    public:
        DataBase()
        {
            const char* path = "Maindb.db";

            int rc = sqlite3_open(path, &db);

            if (rc != SQLITE_OK){
                // QString errorMSG = QString("Failed to open database!\nError: %1").arg(sqlite3_errmsg(db));
                // QMessageBox::critical(nullptr, "Database Error", errorMSG);
                exit(1);
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