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
    string sql ="INSERT INTO Restaurants "
    "(ManagerID, Name, address, active, PrepTime, PhoneNumber, Description, shippingCost) " 
    "VALUES (" +
    to_string(ManagerID) + ", '" +
    Name + "', '" +
    address + "', " +
    to_string(active) + ", " +
    to_string(PrepTime) + ", '" +
    PhoneNumber + "', '" +
    Description + "', " +
    to_string(shippingCost) + ");";

    
    char* messageError;
    int exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        std::cerr << "Error Insert: " << messageError << std::endl;
        sqlite3_free(messageError);
    } 
    else {
        std::cout << "Records created successfully!" << std::endl;
    }
}



void RestaurantDAO:: DeleateRestuarant(int ID)
{
    string sql = "DELETE FROM Restaurants WHERE ID = " + to_string(ID) + ";";
    sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
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
    return ResturantLIST;
}