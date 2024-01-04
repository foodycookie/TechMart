#include "DBManager_Customer.h"
#include<iostream>
using std::cout;
using std::cerr;

void DBManager_Customer::insertDataCustomer(sqlite3* DB, char* messageError, string sql, int exit, string username, string password, string cash) {
    sql = "INSERT INTO CUSTOMER(USERNAME, PASSWORD, CASH) VALUES('"+ username +"', '"+ password +"', '"+ cash +"');";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) 
    {
        cerr << "\nError register!\n";
        sqlite3_free(messageError);
    }
    else
        cout << "\nRegister successful!\n";
}

void DBManager_Customer::getAllCustomerData(sqlite3* DB) {
    query = "SELECT * FROM CUSTOMER;";

    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

void DBManager_Customer::updateDataCustomer(sqlite3* DB, char* messageError, string sql, int exit, string id, string username, string password, string cash) {
    sql = "UPDATE CUSTOMER SET USERNAME = '"+ username +"', PASSWORD = '"+ password +"', CASH = '"+ cash +"' WHERE CUSTOMER_ID = '"+ id +"';";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) 
    {
        cerr << "\nError update!\n";
        sqlite3_free(messageError);
    }
    else
        cout << "\nUpdate data successfully!\n";
}

void DBManager_Customer::deleteDataCustomer(sqlite3* DB, char* messageError, string sql, int exit, string id) {
    sql = "DELETE FROM CUSTOMER WHERE CUSTOMER_ID = '"+ id +"';";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) 
    {
        cerr << "\nError delete!\n";
        sqlite3_free(messageError);
    }
    else
        cout << "\nDelete data successfully!\n";
}

bool DBManager_Customer::checkIfCustomerLoginValid(sqlite3* DB, string username, string password) {
    query = "SELECT USERNAME, PASSWORD FROM CUSTOMER WHERE USERNAME = '"+ username +"' AND PASSWORD = '"+ password +"';";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW) 
    {
        //Valid
        sqlite3_finalize(stmt);
        return true;
    } 
    else if (exit == SQLITE_DONE) 
    {
        //Not valid
        sqlite3_finalize(stmt);
        return false;
    }
    else
    {
        sqlite3_finalize(stmt);
        return false;
    }
}

Customer DBManager_Customer::getCustomerDataBasedOnUsername(sqlite3* DB, string username) {
    int storedCustomerId;
    string storedUsername;
    string storedPassword;
    double storedCash;

    query = "SELECT * FROM CUSTOMER WHERE USERNAME = '"+ username +"';";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW)
    {
        storedCustomerId = sqlite3_column_int(stmt, 0);
        storedUsername = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));;
        storedPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));;
        storedCash = sqlite3_column_double(stmt, 3);
    }

    Customer customer = Customer(storedCustomerId, storedUsername, storedPassword, storedCash);

    sqlite3_finalize(stmt);
    
    return customer;
}

Customer DBManager_Customer::getCustomerDataBasedOnId(sqlite3* DB, string id) {
    int storedCustomerId;
    string storedUsername;
    string storedPassword;
    double storedCash;

    query = "SELECT * FROM CUSTOMER WHERE CUSTOMER_ID = '"+ id +"';";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW)
    {
        storedCustomerId = sqlite3_column_int(stmt, 0);
        storedUsername = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));;
        storedPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));;
        storedCash = sqlite3_column_double(stmt, 3);
    }

    Customer customer = Customer(storedCustomerId, storedUsername, storedPassword, storedCash);

    sqlite3_finalize(stmt);

    return customer;
}

bool DBManager_Customer::checkIfAnyCustomerExists(sqlite3* DB) {
    query = "SELECT * FROM CUSTOMER;";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW) 
    {
        //Customer found
        sqlite3_finalize(stmt);
        return true;
    } 
    else if (exit == SQLITE_DONE) 
    {
        //Customer not found
        sqlite3_finalize(stmt);
        return false;
    }
    else
    {
        sqlite3_finalize(stmt);
        return false;
    }
}

bool DBManager_Customer::checkIfAnyCustomerExistsBasedOnId(sqlite3* DB, string id) {
    query = "SELECT * FROM CUSTOMER WHERE CUSTOMER_ID = '"+ id +"';";

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