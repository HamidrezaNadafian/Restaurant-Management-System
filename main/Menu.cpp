#include "Menu.h"

string Food :: FoodOrDrink() const{ return "Food";}

int Food :: CookOrVol() const{ return CookTime;}

void Food :: displayInfo() const{
    cout << "Name : " << name << "\n" << "description : " << description <<"  Price : " << price;
    cout << "\nCook Time : " << CookTime <<"   ";

    if(available) cout << "\x1b[32m" << "Available\n" << "\x1b[0m" << endl;
    else cout << "\x1b[31m" << "NotAvailable\n" << "\x1b[0m" << endl;
}

string Drink :: FoodOrDrink() const{ return "Drink";}

int Drink :: CookOrVol() const{ return volume;}

void Drink :: displayInfo() const{
    cout << "Name : " << name << "\n" << "description : " << description <<"  Price : " << price;
    cout << "\nvolume : " << volume <<"   ";

    if(available) cout << "\x1b[32m" << "Available\n" << "\x1b[0m" << endl;
    else cout << "\x1b[31m" << "NotAvailable\n" << "\x1b[0m" << endl;
}

