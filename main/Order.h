#ifndef ORDER_H
#define ORDER_H

#include "lib.h"

class Order {
    private:
        int id;
        int UserID;
        int restaurantId;
        string RestaurantName;
        double Price;
        string status;
    public:
        Order(int id, int UserID , int restaurantId, string RestaurantName , double Price, string status)
            : id(id), UserID(UserID),restaurantId(restaurantId), RestaurantName(RestaurantName) , Price(Price), status(status) {}

        void displayInfo()
        {
            cout << "RestaurantName : " << RestaurantName <<"\n" << "Price : " << Price <<"\n" << "status : ";

            if(status == "Delivered")cout << "\x1b[32m" << status << "\x1b[0m" << endl;
            else cout << "\x1b[33m" << status << "\x1b[0m" << endl;
        }

        int getID(){
            return id;
        }
};

class OrderItem {
    private:
        int ID;
        int OrderID;
        string ItemName;
        int Quantity;
        double Price;
        
    public:
        OrderItem(int ID , int OrderID , string ItemName , int Quantity, double Price)
            : ID(ID), OrderID(OrderID), ItemName(ItemName) , Quantity(Quantity), Price(Price) {}

        void displayInfo()
        {
            cout << "Name : " << ItemName<<"\n";
            cout << "Quantity : " << Quantity<<"\n"; 
            cout << "Price : " << Price<<"\n"; 
            
        }
};

#endif