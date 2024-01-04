#include "DBManager_Supplier.h"
#include<iostream>
using std::cout;
using std::cerr;

void DBManager_Supplier::insertDataSupplier(sqlite3* DB, char* messageError, string sql, int exit, string name, string contactInfo) {
    sql = "INSERT INTO SUPPLIER(NAME, CONTACT_INFO) VALUES('"+ name +"', '"+ contactInfo +"');";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) 
    {
        cerr << "\nError insert!\n";
        sqlite3_free(messageError);
    }
    else
        cout << "\nInsert successful!\n";
}

void DBManager_Supplier::getAllSupplierData(sqlite3* DB) {
    query = "SELECT * FROM SUPPLIER;";

    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

void DBManager_Supplier::updateDataSupplier(sqlite3* DB, char* messageError, string sql, int exit, string id, string name, string contactInfo) {
    sql = "UPDATE SUPPLIER SET NAME = '"+ name +"', CONTACT_INFO = '"+ contactInfo +"' WHERE SUPPLIER_ID = '"+ id +"';";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) 
    {
        cerr << "\nError update!\n";
        sqlite3_free(messageError);
    }
    else
        cout << "\nUpdate data successfully!\n";
}

void DBManager_Supplier::deleteDataSupplier(sqlite3* DB, char* messageError, string sql, int exit, string id) {
    sql = "DELETE FROM SUPPLIER WHERE SUPPLIER_ID = '"+ id +"';";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) 
    {
        cerr << "\nError delete!\n";
        sqlite3_free(messageError);
    }
    else
        cout << "\nDelete data successfully!\n";
}

string DBManager_Supplier::getSupplierNamwBasedOnId(sqlite3* DB, string id) {
    string storedname;

    query = "SELECT * FROM SUPPLIER WHERE SUPPLIER_ID = '"+ id +"';";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW)
    {
        storedname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));;
    }

    sqlite3_finalize(stmt);

    return storedname;
}

bool DBManager_Supplier::checkIfSupplierUnique(sqlite3* DB, string name) {
    query = "SELECT NAME FROM SUPPLIER WHERE NAME = '"+ name +"';";

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

bool DBManager_Supplier::checkIfAnySupplierExists(sqlite3* DB) {
    query = "SELECT * FROM SUPPLIER;";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW) 
    {
        //Supplier found
        sqlite3_finalize(stmt);
        return true;
    } 
    else if (exit == SQLITE_DONE) 
    {
        //Supplier not found
        sqlite3_finalize(stmt);
        return false;
    }
    else
    {
        sqlite3_finalize(stmt);
        return false;
    }
}

bool DBManager_Supplier::checkIfSupplierExistsBasedOnId(sqlite3* DB, string id) {
    query = "SELECT * FROM SUPPLIER WHERE SUPPLIER_ID = '"+ id +"';";

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