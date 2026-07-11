#ifndef RESTAURANT_H
#define RESTAURANT_H

#include "lib.h"

class Restaurant
{   
    private:
        int id;
        int ManagerID;
        string name;
        string adress;
        int active;
        int PrepTime;
        string PhoneNumber;
        string Description;
        double shippingCost;

    public:
        Restaurant(int id , int ManagerID , string nm , string adr , int act , int apt , string PN , string Des , double shippingCost):
        id(id) , ManagerID(ManagerID) , name(nm) , adress(adr) , active(act) , PrepTime(apt) , PhoneNumber(PN) , Description(Des) , shippingCost(shippingCost){}

        void displayInfo();
        bool getActive();
        int getID();
        int getManagerID(){
            return ManagerID;
        }
        string getName(){
            return name;
        }
        string getPhone(){
            return PhoneNumber;
        }
        
        string getDescription() {
            return Description;
        }

        string getAddress(){
            return adress;
        }

        double getshippingCost() { 
            return shippingCost; 
        }
        
};

#endif