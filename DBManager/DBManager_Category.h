#pragma once

#include<string>
using std::string;
#include "../Sqlite3/sqlite3.h"
#include "DBManager.h"
#include "../Class/Category.h"

class DBManager_Category  : public DBManager {
    public:
    void insertDataCategory(sqlite3* DB, char* messageError, string sql, int exit, string type);

    void getAllCategoryData(sqlite3* DB);

    void updateDataCategory(sqlite3* DB, char* messageError, string sql, int exit, string id, string type);

    void deleteDataCategory(sqlite3* DB, char* messageError, string sql, int exit, string id);
    
    string getCategoryNameBasedOnId(sqlite3* DB, string id);

    bool checkIfCategoryUnique(sqlite3* DB, string type);

    bool checkIfAnyCategoryExists(sqlite3* DB);

    bool checkIfCategoryExistsBasedOnId(sqlite3* DB, string id);
};