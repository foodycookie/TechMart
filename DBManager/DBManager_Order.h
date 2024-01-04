#pragma once

#include<string>
using std::string;
#include "../Sqlite3/sqlite3.h"
#include "DBManager.h"
#include "../Class/Order.h"

class DBManager_Order  : public DBManager {
    public:
    void insertDataOrder(sqlite3* DB, char* messageError, string sql, int exit, string customerID, string productID, string quantity, string totalPrice);

    void getAllOrder(sqlite3* DB);
    
    void updateDataOrder(sqlite3* DB, char* messageError, string sql, int exit, string id, string customerID, string productID, string quantity, string totalPrice);

    void deleteDataOrder(sqlite3* DB, char* messageError, string sql, int exit, string id);

    void getAllOrderBasedOnCustomer(sqlite3* DB, string id);

    Order getOrderDataBasedOnId(sqlite3* DB, string id);

    void getMostPopularItem(sqlite3* DB);

    bool checkIfAnyOrderExistsBasedOnId(sqlite3* DB, string id);

    bool checkIfAnyOrderExistsBasedOnIdAndCustomerId(sqlite3* DB, string id, string customerId);

    bool checkIfAnyOrderExistsBasedOnCustomerId(sqlite3* DB, string customerId);

    bool checkIfAnyOrderExists(sqlite3* DB);
};