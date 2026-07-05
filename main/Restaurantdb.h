#ifndef Restaurantdb_H
#define Restaurantdb_H

#include "lib.h"
#include "sqlite3.h"
#include "Restaurant.h"
#include "OpenDataBase.h"

class RestaurantDAO
{
    private :
        sqlite3* db;
        
    public :
        RestaurantDAO(DataBase& DB): db(DB.getdb()){}
        void exec(const string& s);

        void CreateRestaurantTable();
        void AddRestaurant(string Name , int ManagerID ,string address , int active , int PrepTime , string PhoneNumber , string Description);
        void DeleateRestuarant(int ID);

        void UpdateINFO(int ID , int NumberType , string newUpdate);

        vector<Restaurant> getRestaurants();


};

#endif