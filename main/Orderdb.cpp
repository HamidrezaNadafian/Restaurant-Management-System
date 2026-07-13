#include "Orderdb.h"


void OrderDAO :: exec(const string& s)
{
    char* messageError;
    int exit = sqlite3_exec(db, s.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << "Error Insert: " << messageError << endl;
        sqlite3_free(messageError);
    } 

}

void OrderDAO :: CreateOrderTable()
{
    string Table = "CREATE TABLE IF NOT EXISTS Orders ("
                    "ID INTEGER PRIMARY KEY AUTOINCREMENT, UserID INTEGER ,restaurantId INTEGER, ResturantName TEXT ,Price REAL, "
                    "status Text ,UserLevel TEXT);";

    exec(Table);    
}

int OrderDAO :: AddOrder(int UserID , int restaurantId, string ResturantName ,double Price, string status , string UserLevel)
{
    string sql = "INSERT INTO Orders (UserID , restaurantId, ResturantName ,Price , status , UserLevel) VALUES ('"
    + to_string(UserID) + "', '" + to_string(restaurantId) + "', '" + ResturantName + "', '" + to_string(Price) + "', '" + status + "', '" + UserLevel + "');";
    
    char* messageError;
    int exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
    

    if (exit != SQLITE_OK) {
        sqlite3_free(messageError);
    } 
    
    return sqlite3_last_insert_rowid(db);
}

void OrderDAO :: UpdateStatusOrder (string status , int ID)
{
    string sql = "UPDATE Orders SET status = '" + status + "' WHERE ID = " + to_string(ID) + ";";
    sqlite3_exec(db, sql.c_str(), NULL, NULL, NULL);
}


vector<Order> OrderDAO :: AllOrders(string RestaurantOrUser ,  int ID)
{

    string sql = "SELECT * FROM Orders WHERE " + RestaurantOrUser + " = " + to_string(ID) + ";";
    sqlite3_stmt* stmt;
    vector<Order> Orders;; 

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int ID = sqlite3_column_int(stmt, 0);
            
            int UserID = sqlite3_column_int(stmt, 1);

            int RestaurantId = sqlite3_column_int(stmt, 2);

            string ResturantName =(const char*)(sqlite3_column_text(stmt, 3)); // (const char*) C-STYLE

            double Price = sqlite3_column_double(stmt, 4);

            string status = (const char*)(sqlite3_column_text(stmt, 5));

            string UserLevel = (const char*)(sqlite3_column_text(stmt, 6));
            
            Order ord(ID , UserID , RestaurantId , ResturantName , Price , status , UserLevel);
            Orders.push_back(ord);
        }
    }
    sqlite3_finalize(stmt);
    return Orders;
}

int OrderDAO :: getTotalOrders()
{
        sqlite3_stmt* stmt;
        const char* sql = "SELECT COUNT(*) FROM Orders;";
        int row_count = -1;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            
            
            if (sqlite3_step(stmt) == SQLITE_ROW) {
            
                row_count = sqlite3_column_int(stmt, 0);
                
            }
        } 

        else{
            cerr << "Error : " << sqlite3_errmsg(db) << endl;
            
        }
        
            
        sqlite3_finalize(stmt);
        return row_count;
    
}

double OrderDAO :: getTotalPrice()
{
    const char* sql = "SELECT SUM(Price) FROM Orders;";
    sqlite3_stmt* stmt;
    double total_sum = -1;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
                
             
        if (sqlite3_step(stmt) == SQLITE_ROW) {    
            total_sum = sqlite3_column_double(stmt, 0);            
            
        }
    } 

    else{
        cerr << "Error : " << sqlite3_errmsg(db) << endl;   
    }
    sqlite3_finalize(stmt);
    return total_sum;
}


void OrderItemsDAO :: CreateOrderItemsTable()
{
    
    string Table = "CREATE TABLE IF NOT EXISTS OrderItems ("
                   "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "OrderID INTEGER, "
                   "ItemName TEXT, "
                   "Quantity INTEGER, "
                   "Price REAL, "
                   "FOREIGN KEY(OrderID) REFERENCES Orders(ID));";
    exec(Table);    
}


void OrderItemsDAO :: exec(const string& s)
{
    char* messageError;
    int exit = sqlite3_exec(db, s.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << "Error Insert: " << messageError << endl;
        sqlite3_free(messageError);
    } 

}

void OrderItemsDAO :: AddOrderItem(int OrderID, string ItemName, int Quantity, double Price)
{
    string sql = "INSERT INTO OrderItems (OrderID, ItemName, Quantity, Price) VALUES ("
    + to_string(OrderID) + ", '" + ItemName + "', " + to_string(Quantity) + ", " + to_string(Price) + ");";
    
    exec(sql);
}

vector<OrderItem> OrderItemsDAO :: GetItemsForOrder(int OrderID)
{
    string sql = "SELECT * FROM OrderItems WHERE OrderID = " + to_string(OrderID) + ";";
    sqlite3_stmt* stmt;
    vector<OrderItem> items;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int ID = sqlite3_column_int(stmt, 0);
            int OrderID = sqlite3_column_int(stmt, 1);
            string ItemName = (const char*)(sqlite3_column_text(stmt, 2));
            int Quantity = sqlite3_column_int(stmt, 3);
            double Price = sqlite3_column_double(stmt, 4);

            OrderItem item(ID, OrderID, ItemName, Quantity, Price);
            items.push_back(item);
        }
    }
    sqlite3_finalize(stmt);
    return items;
}