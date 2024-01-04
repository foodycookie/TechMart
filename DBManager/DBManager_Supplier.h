#pragma once

#include<string>
using std::string;
#include "../Sqlite3/sqlite3.h"
#include "DBManager.h"
#include "../Class/Supplier.h"

class DBManager_Supplier  : public DBManager {
    public:   
    void insertDataSupplier(sqlite3* DB, char* messageError, string sql, int exit, string name, string contactInfo);

    void getAllSupplierData(sqlite3* DB);

    void updateDataSupplier(sqlite3* DB, char* messageError, string sql, int exit, string id, string name, string contactInfo);

    void deleteDataSupplier(sqlite3* DB, char* messageError, string sql, int exit, string id);
    
    string getSupplierNamwBasedOnId(sqlite3* DB, string id);

    bool checkIfSupplierUnique(sqlite3* DB, string name);

    bool checkIfAnySupplierExists(sqlite3* DB);

    bool checkIfSupplierExistsBasedOnId(sqlite3* DB, string id);
};