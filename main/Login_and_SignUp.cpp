#include "lib.h"

#include "sqlite3.h"

#include "OpenDataBase.h"

#include "SecretUserForAdmin.cpp"

class LOGINDAO
{
    private:
        sqlite3* db;

    public :
        LOGINDAO(DataBase& DB): db(DB.getdb()){}

        void exec(const string& s)
        {
            char* messageError;
            int exit = sqlite3_exec(db, s.c_str(), NULL, 0, &messageError);
            if (exit != SQLITE_OK) {
                cerr << "Error Insert: " << messageError << endl;
                sqlite3_free(messageError);
            } 
        }

        void CreateLOGINTable()
        {
            string sql = "CREATE TABLE IF NOT EXISTS users ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "Username TEXT UNIQUE NOT NULL,"
                      "Password TEXT NOT NULL,"
                      "Role TEXT NOT NULL);";

            exec(sql);    
        }

        bool RegisterUser(const string username, const string password , string UserRoles)
        {

            const char* sql = "INSERT INTO users (Username, Password , Role) VALUES (?, ? , ?);";//const

            sqlite3_stmt* stmt;

            sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

            
            sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, UserRoles.c_str(), -1, SQLITE_TRANSIENT);

            int rc = sqlite3_step(stmt);

            sqlite3_finalize(stmt);

            if(rc == SQLITE_DONE){
                cout << "You have successfully registered!!!!!!\n";
                return 1;
            }
            else{
                cout << "The username is duplicated.\n" << sqlite3_errmsg(db) <<"\n";
                return 0;
            }
            return 1;
        }

        bool IsUsernameTrue(int rc)
        {
            return(rc == SQLITE_ROW);
        }

        bool LoginUser(const string username , const string password)
        {
            const char* sql = "SELECT Password FROM users WHERE Username = ?;";// const
            sqlite3_stmt* stmt;

            sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

            sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
            
            int rc = sqlite3_step(stmt);
            
            if(IsUsernameTrue(rc)){
                
                

                if( (const char*) sqlite3_column_text(stmt, 0) == password) {
                    return 1;
                }

                else{
                    cout <<"PASSWORD is WRONG!\n";
                    return 0;
                }
            }
            else{
                cout << "USERNAME Not Found \n";
                return 0;
            }
           
        }

        int getUserIdByUsername(string& username){

            string sql = "SELECT id FROM users WHERE username = ?;";
            sqlite3_stmt* stmt;

            sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

            sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);

            int userId;
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                userId = sqlite3_column_int(stmt, 0); 
            }

            sqlite3_finalize(stmt);
            return userId;
        }

        int getTotalUsers()
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

};

