#ifndef MEMBERSHIPLEVEL_H
#define MEMBERSHIPLEVEL_H

#include "lib.h"
// #include <QString>

class MembershipLevel
{
    public : 
        virtual ~MembershipLevel() {};
        virtual string getLevelName() const = 0;
        virtual double getDiscount() const = 0;
        virtual double getShippingCostdisc() const = 0;
        virtual double getPoints() const = 0;
        virtual int getCoupons() const = 0;

        virtual string getColorCode() const = 0;
        
        virtual string getNextLevelName() const { return ""; }
        virtual string getNextLevelColorCode() const { return ""; }
        virtual int getTargetPoints() const { return 0; }
        
};

class NormalLevel : public MembershipLevel
{
    public : 
        virtual string getLevelName() const override { return "Normal";}
        virtual double getDiscount() const override {return 0.0 ;}
        virtual double getShippingCostdisc() const override {return 1.0 ;}
        virtual double getPoints() const override {return 1.0 ;}
        virtual int getCoupons() const override {return 0; }

        string getColorCode() const override {return "#7f8c8d"; }
        string getNextLevelName() const override { return "Silver"; }
        string getNextLevelColorCode() const override { return "#bdc3c7"; }
        int getTargetPoints() const override { return 100; }
};

class SilverLevel : public MembershipLevel
{
    public : 
        virtual string getLevelName() const override { return "Silver" ;}
        virtual double getDiscount() const override {return 0.05 ;}
        virtual double getShippingCostdisc() const override {return 0.75 ;}
        virtual double getPoints() const override {return 1.2 ;}
        virtual int getCoupons() const override {return 1; }

        string getColorCode() const override {return "#bdc3c7"; }
        string getNextLevelName() const override { return "Gold"; }
        string getNextLevelColorCode() const override { return "#f1c40f"; }
        int getTargetPoints() const override { return 300; }
};

class GoldLevel : public MembershipLevel
{   
    public : 
        virtual string getLevelName() const override { return "Gold";}
        virtual double getDiscount() const override {return 0.1 ;}
        virtual double getShippingCostdisc() const override {return 0.5 ;}
        virtual double getPoints() const override {return 1.5 ;}
        virtual int getCoupons() const override {return 2; }

        string getColorCode() const override {return "#f1c40f"; }
        string getNextLevelName() const override { return "VIP"; }
        string getNextLevelColorCode() const override { return "#8e44ad"; }
        int getTargetPoints() const override { return 700; }
};

class VIPLevel : public MembershipLevel
{
    public : 
        virtual string getLevelName() const override { return "VIP" ;}
        virtual double getDiscount() const override {return 0.15 ;}
        virtual double getShippingCostdisc() const override {return 0.0 ;}
        virtual double getPoints() const override {return 2.0 ;}
        virtual int getCoupons() const override {return 3; }
        
        string getColorCode() const override {return "#8e44ad"; }

};

#endif