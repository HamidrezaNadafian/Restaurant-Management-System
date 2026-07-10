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
};

class NormalLevel : public MembershipLevel
{
    public : 
        virtual string getLevelName() const override { return "Normal";}
        virtual double getDiscount() const override {return 0.0 ;}
        virtual double getShippingCostdisc() const override {return 1.0 ;}
        virtual double getPoints() const override {return 1.0 ;}
};

class SilverLevel : public MembershipLevel
{
    public : 
        virtual string getLevelName() const override { return "Silver" ;}
        virtual double getDiscount() const override {return 0.05 ;}
        virtual double getShippingCostdisc() const override {return 0.75 ;}
        virtual double getPoints() const override {return 1.2 ;}
};

class GoldLevel : public MembershipLevel
{   
    public : 
        virtual string getLevelName() const override { return "Gold";}
        virtual double getDiscount() const override {return 0.1 ;}
        virtual double getShippingCostdisc() const override {return 0.5 ;}
        virtual double getPoints() const override {return 1.5 ;}
};

class VIPLevel : public MembershipLevel
{
    public : 
        virtual string getLevelName() const override { return "VIP" ;}
        virtual double getDiscount() const override {return 0.15 ;}
        virtual double getShippingCostdisc() const override {return 0.0 ;}
        virtual double getPoints() const override {return 2.0 ;}
};

#endif