#ifndef MENUDB_H
#define MENUDB_H

#include "Menu.h"
#include "sqlite3.h"
#include "OpenDataBase.h"

class MenuItemDAO
{
    private : 
        sqlite3 *db;
    public:
        MenuItemDAO(DataBase &DB):db(DB.getdb()){}

        void exec(const string& s);

        void CreateTable();

        void AddMenuItem(int RestaurantID , string Name , string Description , double Price , int available , string FoodOrDrink , int CookOrVol);
        void DeleateMenuItem(int ID);
        void UpdateItem(int ID , string name , string disc , double price , int available , int CookOrVol);

        vector<unique_ptr<MenuItem>> MenuForRestaurant(int RestaurantId);

};

#endif