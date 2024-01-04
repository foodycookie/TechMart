#pragma once

#include<string>
using std::string;
#include "../Sqlite3/sqlite3.h"
#include "DBManager.h"
#include "../Class/Product.h"

class DBManager_Product  : public DBManager {
    public:
    void insertDataProduct(sqlite3* DB, char* messageError, string sql, int exit, string supplierID, string categoryID, string name, string price);

    void getAllProductData(sqlite3* DB);

    void updateDataProduct(sqlite3* DB, char* messageError, string sql, int exit, string id, string supplierID, string categoryID, string name, string price);

    void deleteDataProduct(sqlite3* DB, char* messageError, string sql, int exit, string id);
    
    string getProductNameBasedOnId(sqlite3* DB, string id);

    bool checkIfProductUnique(sqlite3* DB, string name);

    bool checkIfAnyProductExists(sqlite3* DB);

    bool checkIfAnyProductExistsBasedOnId(sqlite3* DB, string id);
};