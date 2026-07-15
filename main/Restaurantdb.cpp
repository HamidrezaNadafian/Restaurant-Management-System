#include "Restaurantdb.h"

void RestaurantDAO :: exec(const string& s)
{
    char* messageError;
    int exit = sqlite3_exec(db, s.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << "Error Insert: " << messageError << endl;
        sqlite3_free(messageError);
    } 
}

void RestaurantDAO:: CreateRestaurantTable()
{
    string Table = "CREATE TABLE IF NOT EXISTS Restaurants ("
                    "ID INTEGER PRIMARY KEY AUTOINCREMENT, ManagerID INTEGER, Name TEXT, address TEXT, "
                    "active INTEGER, PrepTime INTEGER, PhoneNumber TEXT, Description TEXT, shippingCost REAL);";

    exec(Table);    
}

void RestaurantDAO:: AddRestaurant(string Name , int ManagerID ,string address , int active , int PrepTime , string PhoneNumber , string Description , double shippingCost)
{
    sqlite3_stmt* stmt;
    string sql = "INSERT INTO Restaurants (ManagerID, Name, address, active, PrepTime, PhoneNumber, Description, shippingCost) "
                 "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, ManagerID);
        sqlite3_bind_text(stmt, 2, Name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, address.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 4, active);
        sqlite3_bind_int(stmt, 5, PrepTime);
        sqlite3_bind_text(stmt, 6, PhoneNumber.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 7, Description.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 8, shippingCost);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            qDebug() << "Error Insert Restaurant:" << sqlite3_errmsg(db);
        }
    }

    else
    {
        qDebug() << "Prepare failed:" << sqlite3_errmsg(db);
    }

    sqlite3_finalize(stmt);
}



void RestaurantDAO:: DeleateRestuarant(int ID)
{
    sqlite3_stmt* stmt;
    string sql = "DELETE FROM Restaurants WHERE ID = ?;";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, ID);
        sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
}


void RestaurantDAO :: UpdateINFO(int ID , int NumberType , string newUpdate)
{
    string arry[7] = {"Name" , "address" , "PrepTime" , "PhoneNumber" , "Description" , "active" , "shippingCost"};
    
    sqlite3_stmt* stmt;
    string sql = "UPDATE Restaurants SET " + arry[NumberType] + " = ? WHERE ID = ?;";

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, newUpdate.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, ID);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cout << "Error!";
    } 
    
    sqlite3_finalize(stmt);

}

vector<Restaurant> RestaurantDAO :: getRestaurants()
{
    const char* sql = "SELECT * FROM Restaurants;"; // hafeze , c-style , c_str
    sqlite3_stmt* stmt;
    vector<Restaurant> ResturantLIST;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            int ID = sqlite3_column_int(stmt, 0);

            int ManagerID = sqlite3_column_int(stmt, 1);

            string Name = (const char*)(sqlite3_column_text(stmt, 2));
            string address = (const char*)(sqlite3_column_text(stmt, 3));

            int active = sqlite3_column_int(stmt, 4);
            int PrepTime = sqlite3_column_int(stmt, 5);

            string PhoneNumber = (const char*)(sqlite3_column_text(stmt, 6));
            string Description = (const char*)(sqlite3_column_text(stmt, 7)); // unsigned char*

            double shippingCost = sqlite3_column_double(stmt, 8);

            Restaurant rest(ID , ManagerID , Name , address , active , PrepTime , PhoneNumber , Description , shippingCost);
            ResturantLIST.push_back(rest);
        }
    }
    sqlite3_finalize(stmt);
    
    return ResturantLIST;
}