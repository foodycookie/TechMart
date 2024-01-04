#pragma once

#include<string>
#include<vector>
using std::vector;
using std::string;
#include "../Sqlite3/sqlite3.h"
#include "DBManager.h"
#include "../Class/Inventory.h"
#include "../Class/Product.h"

class DBManager_Inventory  : public DBManager {
    public:
    // void insertDataInventory(sqlite3* DB, char* messageError, string sql, int exit, string productID, string quantity);

    void getAllInventoryData(sqlite3* DB);

    void updateDataInventory(sqlite3* DB, char* messageError, string sql, int exit, string id, string quantity);

    // void deleteDataInventory(sqlite3* DB, char* messageError, string sql, int exit, string id);

    Product getProductIDCategoryNamePriceQuantity(sqlite3* DB, string id);

    void listOutProductIDCategoryNamePriceQuantity(sqlite3* DB);

    int getInventoryQuantityBasedOnProductId(sqlite3* DB, string id);

    vector<Product> getListOfProduct(sqlite3* DB);

    bool checkIfAnyInventoryExistsBasedOnId(sqlite3* DB, string id);

    bool checkIfAnyBuyableExists(sqlite3* DB);
};