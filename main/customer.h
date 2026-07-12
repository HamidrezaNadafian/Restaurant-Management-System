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
        int coupons = 0;
        string badges = "";

    public :
        Customer(int ID, string Username, int Points):
            ID(ID) , Username(Username) , Points(Points){
                CurrentLevel = LevelFactory::getLevel(Points);
            }


        ~Customer() {
            delete CurrentLevel;
        }

        void UpdateLevel();
        void AddPoints(double TotalCost);

        int getID() const {return ID; }
        string getUsername() const {return Username; }
        int getPoints() const {return Points; }
        double getFinalPrice(int Price) const {return Price - Price * CurrentLevel->getDiscount() ;}
        MembershipLevel* getMembership() const {return CurrentLevel; }
        int getCoupons() const {return coupons; }
        void setCoupons(int c) {coupons = c; }
        string getBadges() const { return badges; }
        void setBadges(string b) { badges = b; }
           
};


#endif