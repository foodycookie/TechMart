#include "DBManager_Inventory.h"
#include<iostream>
using std::cout;
using std::cerr;

// void DBManager_Inventory::insertDataInventory(sqlite3* DB, char* messageError, string sql, int exit, string productID, string quantity) {
//     sql = "INSERT INTO INVENTORY(PRODUCT_ID, QUANTITY) VALUES('"+ productID +"', '"+ quantity +"');";

//     exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

//     if (exit != SQLITE_OK) 
//     {
//         cerr << "\nError insert!\n";
//         sqlite3_free(messageError);
//     }
//     else
//         cout << "\nInsert successful!\n";
// }

void DBManager_Inventory::getAllInventoryData(sqlite3* DB) {
    query = "SELECT * FROM INVENTORY;";

    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

void DBManager_Inventory::updateDataInventory(sqlite3* DB, char* messageError, string sql, int exit, string id, string quantity) {
    sql = "UPDATE INVENTORY SET QUANTITY = '"+ quantity +"' WHERE INVENTORY_ID = '"+ id +"';";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) 
    {
        cerr << "\nError update!\n";
        sqlite3_free(messageError);
    }
    else
        cout << "\nUpdate data successfully!\n";
}

// void DBManager_Inventory::deleteDataInventory(sqlite3* DB, char* messageError, string sql, int exit, string id) {
//     sql = "DELETE FROM INVENTORY WHERE PRODUCT_ID = '"+ id +"';";

//     exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

//     if (exit != SQLITE_OK) 
//     {
//         cerr << "\nError delete!\n";
//         sqlite3_free(messageError);
//     }
//     else
//         cout << "\nDelete data successfully!\n";
// }

Product DBManager_Inventory::getProductIDCategoryNamePriceQuantity(sqlite3* DB, string id) {
    int storedId,storedQuantity;
    double storedPrice;
    string storedType,storeName;

    query = "SELECT PRODUCT.PRODUCT_ID, CATEGORY.TYPE, PRODUCT.NAME, PRODUCT.PRICE, INVENTORY.QUANTITY "
    "FROM PRODUCT "
    "JOIN CATEGORY ON PRODUCT.CATEGORY_ID = CATEGORY.CATEGORY_ID "
    "JOIN INVENTORY ON PRODUCT.PRODUCT_ID = INVENTORY.PRODUCT_ID "
    "WHERE PRODUCT.PRODUCT_ID = '"+ id +"' AND INVENTORY.QUANTITY > 0;";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW)
    {
        storedId = sqlite3_column_int(stmt, 0);
        storedType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));;
        storeName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));;
        storedPrice = sqlite3_column_double(stmt, 3);
        storedQuantity = sqlite3_column_int(stmt, 4);
    }

    else
    {
        cerr << "Error fetching product information: " << sqlite3_errmsg(DB) << "\n";
    }
    
    Product product = Product(storedId, storedType, storeName, storedPrice, storedQuantity);

    sqlite3_finalize(stmt);
    
    return product;
}

void DBManager_Inventory::listOutProductIDCategoryNamePriceQuantity(sqlite3* DB) {
    query = "SELECT PRODUCT.PRODUCT_ID, CATEGORY.TYPE, PRODUCT.NAME, PRODUCT.PRICE, INVENTORY.QUANTITY "
    "FROM PRODUCT "
    "JOIN CATEGORY ON PRODUCT.CATEGORY_ID = CATEGORY.CATEGORY_ID "
    "JOIN INVENTORY ON PRODUCT.PRODUCT_ID = INVENTORY.PRODUCT_ID "
    "WHERE INVENTORY.QUANTITY > 0;";

    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

int DBManager_Inventory::getInventoryQuantityBasedOnProductId(sqlite3* DB, string id) {
    int storedQuantity;

    query = "SELECT QUANTITY FROM INVENTORY WHERE PRODUCT_ID = '"+ id +"';";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW)
    {
        storedQuantity = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    
    return storedQuantity;
}

vector<Product> DBManager_Inventory::getListOfProduct(sqlite3* DB) {
    int storedId,storedQuantity;
    double storedPrice;
    string storedType,storeName;
    vector<Product> listOfProduct;

    query = "SELECT PRODUCT.PRODUCT_ID, CATEGORY.TYPE, PRODUCT.NAME, PRODUCT.PRICE, INVENTORY.QUANTITY "
    "FROM PRODUCT "
    "JOIN CATEGORY ON PRODUCT.CATEGORY_ID = CATEGORY.CATEGORY_ID "
    "JOIN INVENTORY ON PRODUCT.PRODUCT_ID = INVENTORY.PRODUCT_ID ;";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    while (exit == SQLITE_ROW)
    {
        storedId = sqlite3_column_int(stmt, 0);
        storedType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));;
        storeName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));;
        storedPrice = sqlite3_column_double(stmt, 3);
        storedQuantity = sqlite3_column_int(stmt, 4);

        Product product(storedId, storedType, storeName, storedPrice, storedQuantity);
        listOfProduct.push_back(product);   

        exit = sqlite3_step(stmt);
    }
    
    sqlite3_finalize(stmt);
    
    return listOfProduct;
}

bool DBManager_Inventory::checkIfAnyInventoryExistsBasedOnId(sqlite3* DB, string id) {
    query = "SELECT * FROM INVENTORY WHERE INVENTORY_ID = '"+ id +"';";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW) 
    {
        //ID found
        sqlite3_finalize(stmt);
        return true;
    } 
    else if (exit == SQLITE_DONE) 
    {
        //ID not found
        sqlite3_finalize(stmt);
        return false;
    }
    else
    {
        sqlite3_finalize(stmt);
        return false;
    }
}

bool DBManager_Inventory::checkIfAnyBuyableExists(sqlite3* DB) {
    query = "SELECT PRODUCT.PRODUCT_ID, CATEGORY.TYPE, PRODUCT.NAME, PRODUCT.PRICE, INVENTORY.QUANTITY "
    "FROM PRODUCT "
    "JOIN CATEGORY ON PRODUCT.CATEGORY_ID = CATEGORY.CATEGORY_ID "
    "JOIN INVENTORY ON PRODUCT.PRODUCT_ID = INVENTORY.PRODUCT_ID "
    "WHERE INVENTORY.QUANTITY > 0;";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW) 
    {
        //Yes
        sqlite3_finalize(stmt);
        return true;
    } 
    else if (exit == SQLITE_DONE) 
    {
        //No
        sqlite3_finalize(stmt);
        return false;
    }
    else
    {
        sqlite3_finalize(stmt);
        return false;
    }
}