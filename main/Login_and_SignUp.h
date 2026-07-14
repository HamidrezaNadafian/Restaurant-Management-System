#ifndef LOGINDAO_H
#define LOGINDAO_H

#include "lib.h"
#include "sqlite3.h"
#include "OpenDataBase.h"
#include "SecretUserForAdmin.h"
#include "customer.h"

struct LevelLog {
    string Username;
    string OldLevel;
    string NewLevel;
    string Date;
};


class LOGINDAO
{
private:
    sqlite3* db;
    
    bool IsUsernameTrue(int rc);

public:
    
    LOGINDAO(DataBase& DB);

    void exec(const string& s);
    void CreateLOGINTable();
    bool RegisterUser(const string username, const string password, string UserRoles, string& message);
    bool LoginUser(const string username, const string password , string& message);
    int getUserIdByUsername(string& username);
    int getTotalUsers();

    Customer* getCustomerByUsername(const string& username);
    bool updateLoyalty(int ID , int points , string level);
    
    string getUsernameById(int id);
    
    void updateCoupons(int userId, int newCouponCount);
    
    vector<Customer*> getAllCustomers();

    void updateBadges(int ID, string NewBadges);

    void AddLevelChange(string Username, string OldLevel, string NewLevel);
    std::vector<LevelLog> getLevelHistory();

    int isFirstTimeUpgrade(string Username);
};


#endif