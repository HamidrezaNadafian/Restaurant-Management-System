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
                

    exec(sql);    
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
    string sql = "SELECT id, loyalty_points, Coupons, Badges FROM users WHERE username = '" + username + "';";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK){
        if (sqlite3_step(stmt) == SQLITE_ROW){
            int ID = sqlite3_column_int(stmt, 0);
            int Points = sqlite3_column_int(stmt, 1);
            int Coupons = sqlite3_column_int(stmt, 2);
            string Badges = (const char*)(sqlite3_column_text(stmt, 3));

            sqlite3_finalize(stmt);

            Customer* customer = new Customer(ID, username, Points);
            customer->setCoupons(Coupons);
            customer->setBadges(Badges);
            
            return customer;
        }
    }
    sqlite3_finalize(stmt);
    return nullptr;
}
bool LOGINDAO::updateLoyalty (int ID , int points , string level)
{
    string query = "UPDATE users SET loyalty_points = " + std::to_string(points) + ", membership_level = '" + level + "' WHERE id = " + to_string(ID) + ";";

    char* message = nullptr;
    int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &message);
    if (rc != SQLITE_OK) {
        sqlite3_free(message);
        return false;
    }   
    return true;
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

void LOGINDAO::updateCoupons(int UserId, int newCouponCount)
{
    string sql = "UPDATE users SET Coupons = " + to_string(newCouponCount) + 
                 " WHERE ID = " + to_string(UserId) + ";";
                 
    char* messageError;
    int exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK) {
        cerr << "Error Updating Coupons: " << messageError << endl;
        sqlite3_free(messageError);
    }
}
vector<Customer*> LOGINDAO::getAllCustomers()
{
    vector<Customer*> CustomersList;
    string sql = "SELECT * FROM Users WHERE Role = 'Customer';"; 
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL) == SQLITE_OK)
    {
        while (sqlite3_step(stmt) == SQLITE_ROW){
            int ID = sqlite3_column_int(stmt, 0);
            string username = (const char*)(sqlite3_column_text(stmt, 1));
            int points = sqlite3_column_int(stmt, 
            4);
            int coupons = sqlite3_column_int(stmt, 6);
            string badgeText = (const char*)(sqlite3_column_text(stmt, 6));

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