#pragma once

#include<string>
using std::string;
#include "../Sqlite3/sqlite3.h"

class DBManager {
    public:
    sqlite3* DB;
    char* messageError;
    sqlite3_stmt* stmt;
    string sql;
    string query;
    int exit;

    DBManager();

    void openDB();

    void closeDB();

    bool checkIfUsernameUnique(sqlite3* DB, string username);

    static int callback(void* data, int argc, char** argv, char** azColName);
};