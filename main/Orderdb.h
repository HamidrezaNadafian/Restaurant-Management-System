#ifndef Orderdb_H
#define Orderdb_H

#include "sqlite3.h"
#include "OpenDataBase.h"
#include "Order.h"

class OrderDAO
{
    private :
        sqlite3* db;

        
    public :
        OrderDAO(DataBase& DB): db(DB.getdb()){}
        void exec(const string& s);
        void CreateOrderTable();
        int AddOrder(int UserID , int restaurantId, string ResturantName ,double Price, string status);

        void UpdateStatusOrder(string status , int ID);

        vector<Order> AllOrders(string RestaurantOrUser ,  int ID);

        int getTotalOrders();
        double getTotalPrice();
};

class OrderItemsDAO
{
    private :
        sqlite3* db;

        
    public :
        OrderItemsDAO(DataBase& DB): db(DB.getdb()){}

        void exec(const string& s);
        void CreateOrderItemsTable();

        void AddOrderItem(int OrderID, string ItemName, int Quantity, double Price);
        
        vector<OrderItem> GetItemsForOrder(int OrderID);

};


#endif