#include "Login_and_SignUp.h"



LOGINDAO::LOGINDAO(DataBase& DB) : db(DB.getdb()) {}

void LOGINDAO::exec(const string& s)
{
    char* messageError;
    int exit = sqlite3_exec(db, s.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << "Error Insert: " << messageError << endl;
        sqlite3_free(messageError);
    } 
}

void LOGINDAO::CreateLOGINTable()
{
    string sql = "CREATE TABLE IF NOT EXISTS users ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "Username TEXT UNIQUE NOT NULL,"
                "Password TEXT NOT NULL,"
                "Role TEXT NOT NULL, "
                "loyalty_points INTEGER DEFAULT 0, "
                "membership_level TEXT DEFAULT 'Normal', "
                "Coupons INTEGER DEFAULT 0, "
                "Badges TEXT DEFAULT '');";
                
    string sqlp = "CREATE TABLE IF NOT EXISTS LevelHistory ("
                    "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "Username TEXT,"
                    "OldLevel TEXT,"
                    "NewLevel TEXT,"
                    "ChangeDate TIMESTAMP DEFAULT CURRENT_TIMESTAMP);" ;

    exec(sql);    
    exec(sqlp);    
}

bool LOGINDAO:: RegisterUser(const string username, const string password, string UserRoles, string& message)   
{
    const char* sql = "INSERT INTO users (Username, Password , Role) VALUES (?, ? , ?);";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, UserRoles.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if(rc == SQLITE_DONE){
        return true;
    }
    else{
        message = "The username is duplicated." ;
        return false;
    }
}

bool LOGINDAO::IsUsernameTrue(int rc)
{
    return(rc == SQLITE_ROW);
}

bool LOGINDAO::LoginUser(const string username, const string password, string& message)
{
    const char* sql = "SELECT Password FROM users WHERE Username = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    
    int rc = sqlite3_step(stmt);
    
    if(IsUsernameTrue(rc)){
        
        string dbPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if(dbPassword == password) {
            sqlite3_finalize(stmt);
            return true;
        }
        else{
            message = "PASSWORD is WRONG!";
            sqlite3_finalize(stmt);
            return false;
        }
    }
    else{
        message =  "USERNAME Not Found ";
        sqlite3_finalize(stmt);
        return false;
    }
}

int LOGINDAO::getUserIdByUsername(string& username){
    string sql = "SELECT id FROM users WHERE username = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

    int UserId = -1; 
    if (sqlite3_step(stmt) == SQLITE_ROW) {
    UserId = sqlite3_column_int(stmt, 0); 
    }

    sqlite3_finalize(stmt);
    return UserId;
}

int LOGINDAO::getTotalUsers()
{
    sqlite3_stmt* stmt;
    const char* sql = "SELECT COUNT(*) FROM users;";
    int row_count = -1;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            row_count = sqlite3_column_int(stmt, 0);
        }
    } 
    else{
        cerr << "Error : " << sqlite3_errmsg(db) << endl;
    }
    sqlite3_finalize(stmt);
    return row_count;
}

Customer* LOGINDAO::getCustomerByUsername(const string& username)
{
    string sql = "SELECT id, loyalty_points, Coupons, Badges FROM users WHERE username = ?;";

    sqlite3_stmt* stmt;
    Customer* customer = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK){
        sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) == SQLITE_ROW){
            int ID = sqlite3_column_int(stmt, 0);
            int Points = sqlite3_column_int(stmt, 1);
            int Coupons = sqlite3_column_int(stmt, 2);

            const char* badgesText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            string Badges = badgesText ? badgesText : "";

            customer = new Customer(ID, username, Points);
            customer->setCoupons(Coupons);
            customer->setBadges(Badges);
        }
    }

    else{
        qDebug() << "Error in getCustomerByUsername:" << sqlite3_errmsg(db);
    }

    sqlite3_finalize(stmt);
    return customer;
}


bool LOGINDAO::updateLoyalty(int ID , int points , string level)
{
    string query = "UPDATE users SET loyalty_points = ?, membership_level = ? WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, points);
        sqlite3_bind_text(stmt, 2, level.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 3, ID);

        if (sqlite3_step(stmt) == SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return true;
        }
    }
    sqlite3_finalize(stmt);
    return false;
}

string LOGINDAO :: getUsernameById(int id)
{
    string sql = "SELECT Username FROM users WHERE id = ?;";
    sqlite3_stmt* stmt;
    string username = "";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            const char* text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            if (text != nullptr) {
                username = text;
            }
        }

    }

    sqlite3_finalize(stmt);
    return username;
}

void LOGINDAO::updateCoupons(int UserId, int NewCouponCount)
{
    string sql = "UPDATE users SET Coupons = ? WHERE ID = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, NewCouponCount);
        sqlite3_bind_int(stmt, 2, UserId);
        sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
}
vector<Customer*> LOGINDAO::getAllCustomers()
{
    vector<Customer*> CustomersList;
    string sql = "SELECT * FROM users;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW){
            int ID = sqlite3_column_int(stmt, 0);
            string username = (const char*)(sqlite3_column_text(stmt, 1));
            int points = sqlite3_column_int(stmt, 4);
            int coupons = sqlite3_column_int(stmt, 6);
            string badgeText = (const char*)(sqlite3_column_text(stmt, 7));

            Customer* customer = new Customer(ID , username , points);
            customer->setCoupons(coupons);
            customer->setBadges(badgeText);

            CustomersList.push_back(customer);
        }
    }
    sqlite3_finalize(stmt);
    return CustomersList;
}

void LOGINDAO::updateBadges(int ID, string NewBadges)
{
    string sql = "UPDATE users SET Badges = '" + NewBadges + "' WHERE ID = " + to_string(ID) + ";";

    char* MessageError;
    int exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &MessageError);
    if (exit != SQLITE_OK) {
        cerr << "Error Updating Badges: " << MessageError << endl;
        sqlite3_free(MessageError);
    }

}

void LOGINDAO :: AddLevelChange(string Username, string OldLevel, string NewLevel)
{
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO LevelHistory (Username, OldLevel , NewLevel) VALUES (?, ? , ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, Username.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, OldLevel.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, NewLevel.c_str(), -1, SQLITE_TRANSIENT);

        sqlite3_step(stmt);
        
    }
    sqlite3_finalize(stmt);
}

vector<LevelLog> LOGINDAO::getLevelHistory()
{
    vector<LevelLog> AllLevelChanges;
    sqlite3_stmt* stmt;

    const char* sql = "SELECT Username, OldLevel, NewLevel, ChangeDate FROM LevelHistory ORDER BY ID DESC"; //

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            LevelLog UserChangeLevel;
            UserChangeLevel.Username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            UserChangeLevel.OldLevel = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            UserChangeLevel.NewLevel =reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            UserChangeLevel.Date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

            AllLevelChanges.push_back(UserChangeLevel);
        }
    }
    sqlite3_finalize(stmt);
    return AllLevelChanges;
}

int LOGINDAO::isFirstTimeUpgrade(string Username)
{
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "SELECT COUNT(*) FROM LevelHistory WHERE Username = ? AND NewLevel != 'Normal'";
    int count = -1;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, Username.c_str(), -1, SQLITE_TRANSIENT);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            count = sqlite3_column_int(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);
    return count ;
}