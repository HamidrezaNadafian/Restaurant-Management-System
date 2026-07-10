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
                "membership_level TEXT DEFAULT 'Normal'"
                ");";

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

    int userId = -1; 
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        userId = sqlite3_column_int(stmt, 0); 
    }

    sqlite3_finalize(stmt);
    return userId;
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
    string sql = "SELECT id, loyalty_points FROM users WHERE username = '" + username + "';";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK){
        if (sqlite3_step(stmt) == SQLITE_ROW){
            int ID = sqlite3_column_int(stmt, 0);
            int Points = sqlite3_column_int(stmt, 1);
            sqlite3_finalize(stmt);

            return new Customer(ID, username, Points);
        }
    }
    sqlite3_finalize(stmt);
    return nullptr;
}
bool LOGINDAO::updateLoyalty (int ID , int points , string level)
{
    string query = "UPDATE users SET loyalty_points = " + std::to_string(points) + ", membership_level = '" + level + "' WHERE id = " + std::to_string(ID) + ";";

    char* message = nullptr;
    int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &message);
    if (rc != SQLITE_OK) {
        sqlite3_free(message);
        return false;
    }   
    return true;
}
