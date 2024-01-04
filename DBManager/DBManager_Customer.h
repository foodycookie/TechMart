#pragma once

#include<string>
using std::string;
#include "../Sqlite3/sqlite3.h"
#include "DBManager.h"
#include "../Class/Customer.h"

class DBManager_Customer  : public DBManager {
    public:
    void insertDataCustomer(sqlite3* DB, char* messageError, string sql, int exit, string username, string password, string cash);

    void getAllCustomerData(sqlite3* DB);

    void updateDataCustomer(sqlite3* DB, char* messageError, string sql, int exit, string id, string username, string password, string cash);

    void deleteDataCustomer(sqlite3* DB, char* messageError, string sql, int exit, string id);
    
    bool checkIfCustomerLoginValid(sqlite3* DB, string username, string password);

    Customer getCustomerDataBasedOnUsername(sqlite3* DB, string username);

    Customer getCustomerDataBasedOnId(sqlite3* DB, string id);

    bool checkIfAnyCustomerExists(sqlite3* DB);

    bool checkIfAnyCustomerExistsBasedOnId(sqlite3* DB, string id);
};