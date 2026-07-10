#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "membershiplevel.h"

class LevelFactory {

    public:
        static MembershipLevel* getLevel(int points) {
            if (points >= 700) return new VIPLevel();
            if (points >= 300) return new GoldLevel();
            if (points >= 100) return new SilverLevel();
            return new NormalLevel();
        }
};


class Customer
{
    private:
        int ID;
        string Username;
        int Points;
        MembershipLevel* CurrentLevel;

    public :
        Customer(int ID, string Username, int Points):
            ID(ID) , Username(Username) , Points(Points){}


        ~Customer() {
            delete CurrentLevel;
        }

        void UpdateLevel();
        void AddPoints(double TotalCost);

        int getID() const {return ID; }
        string getUsername() const {return Username; }
        int getPoints() const {return Points; }
        string getLevelName() const  {return CurrentLevel->getLevelName(); }
        double getFinalPrice(int Price) const {return Price - Price * CurrentLevel->getDiscount() ;}

        
           
};


#endif