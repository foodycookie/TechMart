#pragma once

#include<string>
#include<vector>
using std::string;
using std::vector;
#include "../Sqlite3/sqlite3.h"
#include "DBManager.h"
#include "../Class/Admin.h"

class DBManager_Admin  : public DBManager {
    public:
    void insertDataAdmin(sqlite3* DB, char* messageError, string sql, int exit, string username, string password);

    void getAllAdminData(sqlite3* DB);

    void updateDataAdmin(sqlite3* DB, char* messageError, string sql, int exit, string id, string username, string password);

    void deleteDataAdmin(sqlite3* DB, char* messageError, string sql, int exit, string id);
    
    bool checkIfAdminLoginValid(sqlite3* DB, string username, string password);

    Admin getAdminDataBasedOnUsername(sqlite3* DB, string username);

    string getAdminUsernameBasedOnId(sqlite3* DB, string id);

    bool checkIfAnyAdminExists(sqlite3* DB);

    bool checkIfAnyAdminExistsBasedOnId(sqlite3* DB, string id);
};