#include "Menudb.h"

void MenuItemDAO :: exec(const string& s)
{
    char* messageError;
    int exit = sqlite3_exec(db, s.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << "Error Insert: " << messageError << endl;
        sqlite3_free(messageError);
    } 
}

void MenuItemDAO :: CreateTable()
{
    string Table = "CREATE TABLE IF NOT EXISTS MenuItems ("
                   "ID INTEGER PRIMARY KEY AUTOINCREMENT, RestaurantID INTEGER, Name TEXT, "
                   "Description TEXT, Price REAL, available INTEGER , FoodOrDrink TEXT , CookOrVol INTEGER , IsSpecial INTEGER DEFAULT 0);";

    exec(Table); 
}

void MenuItemDAO :: AddMenuItem(int RestaurantID , string Name , string Description , double Price , int available , string FoodOrDrink , int CookOrVol , int isSpecial)
{
    string sql = "INSERT INTO MenuItems (RestaurantID, Name, Description, Price, available, FoodOrDrink, CookOrVol, IsSpecial) "
                 "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, RestaurantID);
        sqlite3_bind_text(stmt, 2, Name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, Description.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, 4, Price);
        sqlite3_bind_int(stmt, 5, available);
        sqlite3_bind_text(stmt, 6, FoodOrDrink.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 7, CookOrVol);
        sqlite3_bind_int(stmt, 8, isSpecial);

        sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
}

void MenuItemDAO:: DeleateMenuItem(int ID)
{
    string sql = "DELETE FROM MenuItems WHERE ID = " + to_string(ID) + ";";
    sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
}

void MenuItemDAO:: UpdateItem(int ID , string name , string disc , double price , int available , int CookOrVol, int isSpecial)
{
    sqlite3_stmt *stmt;
    string sql = "UPDATE MenuItems SET Name = ?, Description = ?, Price = ?, available = ? , CookOrVol = ?, IsSpecial = ? WHERE ID = ?;";  
    
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);


    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, disc.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, price);
    sqlite3_bind_int(stmt, 4, available);
    sqlite3_bind_int(stmt, 5, CookOrVol);
    sqlite3_bind_int(stmt, 6, isSpecial);
    sqlite3_bind_int(stmt, 7, ID);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

vector<unique_ptr<MenuItem>> MenuItemDAO:: MenuForRestaurant(int RestaurantId)
{
    string sql = "SELECT * FROM MenuItems WHERE RestaurantID = " + to_string(RestaurantId) + ";";
    sqlite3_stmt* stmt;
    vector<unique_ptr<MenuItem>> mnitems;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int ID = sqlite3_column_int(stmt, 0);
            int RestaurantID = sqlite3_column_int(stmt, 1);

            string Name = (const char*)(sqlite3_column_text(stmt, 2));

            string Description =(const char*)(sqlite3_column_text(stmt, 3)); // (const char*) C-STYLE

            double Price = sqlite3_column_double(stmt, 4);
            int available = sqlite3_column_int(stmt, 5);

            string FoodOrDrink = (const char*)(sqlite3_column_text(stmt, 6));

            int CookOrVol = sqlite3_column_int(stmt, 7);
            int isSpecial = sqlite3_column_int(stmt, 8);

            if(FoodOrDrink == "Food"){
                mnitems.push_back(make_unique<Food>(ID, RestaurantID, Name, Description, Price, available, CookOrVol , isSpecial));

            }
            else{
                mnitems.push_back(make_unique<Drink>(ID, RestaurantID, Name, Description, Price, available, CookOrVol , isSpecial));
            }
        }
    }
    sqlite3_finalize(stmt);
    return mnitems;
}