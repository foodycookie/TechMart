#include "DBManager.h"

DBManager::DBManager(): DB(nullptr),messageError(nullptr),sql(""),exit(0) {}

void DBManager::openDB() {
    exit = sqlite3_open("shop.db", &DB);
}

void DBManager::closeDB() {
    sqlite3_close(DB);
}

int DBManager::callback(void* data, int argc, char** argv, char** azColName) {
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
  
    for (i = 0; i < argc; i++) 
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
  
    printf("\n");
    return 0;
}

bool DBManager::checkIfUsernameUnique(sqlite3* DB, string username) {
    query = "SELECT USERNAME FROM CUSTOMER WHERE USERNAME = '"+ username +"' UNION SELECT USERNAME FROM ADMIN WHERE USERNAME = '"+ username +"';";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW) 
    {
        //Username found
        sqlite3_finalize(stmt);
        return false;
    } 
    else if (exit == SQLITE_DONE) 
    {
        //Username not found
        sqlite3_finalize(stmt);
        return true;
    }
    else
    {
        sqlite3_finalize(stmt);
        return false;
    }
}