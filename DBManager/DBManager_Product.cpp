#include "DBManager_Product.h"
#include<iostream>
using std::cout;
using std::cerr;

void DBManager_Product::insertDataProduct(sqlite3* DB, char* messageError, string sql, int exit, string supplierID, string categoryID, string name, string price) {
    sql = "INSERT INTO PRODUCT(SUPPLIER_ID, CATEGORY_ID, NAME, PRICE) VALUES('"+ supplierID +"', '"+ categoryID +"', '"+ name +"', '"+ price +"');";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) 
    {
        cerr << "\nError insert!\n";
        sqlite3_free(messageError);
    }
    else
        cout << "\nInsert successful!\n";
}

void DBManager_Product::getAllProductData(sqlite3* DB) {
    query = "SELECT * FROM PRODUCT;";

    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

void DBManager_Product::updateDataProduct(sqlite3* DB, char* messageError, string sql, int exit, string id, string supplierID, string categoryID, string name, string price) {
    sql = "UPDATE PRODUCT SET SUPPLIER_ID = '"+ supplierID +"', CATEGORY_ID = '"+ categoryID +"', NAME = '"+ name +"', PRICE = '"+ price +"' WHERE PRODUCT_ID = '"+ id +"';";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) 
    {
        cerr << "\nError update!\n";
        sqlite3_free(messageError);
    }
    else
        cout << "\nUpdate data successfully!\n";
}

void DBManager_Product::deleteDataProduct(sqlite3* DB, char* messageError, string sql, int exit, string id) {
    sql = "DELETE FROM PRODUCT WHERE PRODUCT_ID = '"+ id +"';";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) 
    {
        cerr << "\nError delete!\n";
        sqlite3_free(messageError);
    }
    else
        cout << "\nDelete data successfully!\n";
}

string DBManager_Product::getProductNameBasedOnId(sqlite3* DB, string id) {
    string storedName;

    query = "SELECT * FROM PRODUCT WHERE PRODUCT_ID = '"+ id +"';";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW)
    {
        storedName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));;
    }

    sqlite3_finalize(stmt);

    return storedName;
}

bool DBManager_Product::checkIfProductUnique(sqlite3* DB, string name) {
    query = "SELECT NAME FROM PRODUCT WHERE NAME = '"+ name +"';";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW) 
    {
        //Name found
        sqlite3_finalize(stmt);
        return false;
    } 
    else if (exit == SQLITE_DONE) 
    {
        //Name not found
        sqlite3_finalize(stmt);
        return true;
    }
    else
    {
        sqlite3_finalize(stmt);
        return false;
    }
}

bool DBManager_Product::checkIfAnyProductExists(sqlite3* DB) {
    query = "SELECT * FROM PRODUCT;";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW) 
    {
        //Product found
        sqlite3_finalize(stmt);
        return true;
    } 
    else if (exit == SQLITE_DONE) 
    {
        //Product not found
        sqlite3_finalize(stmt);
        return false;
    }
    else
    {
        sqlite3_finalize(stmt);
        return false;
    }
}

bool DBManager_Product::checkIfAnyProductExistsBasedOnId(sqlite3* DB, string id) {
    query = "SELECT * FROM PRODUCT WHERE PRODUCT_ID = '"+ id +"';";

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