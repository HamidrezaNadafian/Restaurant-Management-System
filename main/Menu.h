#ifndef MENU_H
#define MENU_H

#include "lib.h"

class MenuItem
{
    protected:
        int ID;
        int RestaurantID;
        string name;
        string description;
        double price;
        int available;
    public:
        MenuItem(int ID , int RestaurantID , string name , string description , double price , int available):
            ID(ID) , RestaurantID(RestaurantID) , name(name) , description(description) , price(price) , available(available){}

        int getAvailable(){
            return available;
        }
        double getPrice()
        {
            return price;
        }
        int getID()
        {
            return ID;
        }

        string getName()
        {
            return name;
        }

        virtual string FoodOrDrink() const = 0;
        virtual int CookOrVol() const = 0;
        virtual void displayInfo() const = 0;
};

class Food : public MenuItem
{
    private:
        int CookTime;
    public:
        Food(int ID, int RestaurantID, string name, string description, double price, int available, int CookTime)
        : MenuItem(ID, RestaurantID, name, description, price, available), CookTime(CookTime) {}

        string FoodOrDrink() const override;
        int CookOrVol() const override;
        void displayInfo() const override;
        

};

class Drink : public MenuItem
{
    private:
        int volume;
    public:
        Drink(int ID, int RestaurantID, string name, string description, double price, int available, int volume)
        : MenuItem(ID, RestaurantID, name, description, price, available), volume(volume) {}
        
        string FoodOrDrink() const override;
        int CookOrVol() const override;
        void displayInfo() const override;
};

#endif