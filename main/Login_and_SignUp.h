#ifndef LOGINDAO_H
#define LOGINDAO_H

#include "lib.h"
#include "sqlite3.h"
#include "OpenDataBase.h"
#include "SecretUserForAdmin.h"

class LOGINDAO
{
private:
    sqlite3* db;
    
    bool IsUsernameTrue(int rc);

public:
    
    LOGINDAO(DataBase& DB);

    void exec(const string& s);
    void CreateLOGINTable();
    bool RegisterUser(const string username, const string password, string UserRoles);
    bool LoginUser(const string username, const string password);
    int getUserIdByUsername(string& username);
    int getTotalUsers();
};

#endif