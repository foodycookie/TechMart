#include "DBManager_Category.h"
#include<iostream>
using std::cout;
using std::cerr;

void DBManager_Category::insertDataCategory(sqlite3* DB, char* messageError, string sql, int exit, string type) {
    sql = "INSERT INTO CATEGORY(TYPE) VALUES('"+ type +"');";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) 
    {
        cerr << "\nError insert!\n";
        sqlite3_free(messageError);
    }
    else
        cout << "\nInsert successful!\n";
}

void DBManager_Category::getAllCategoryData(sqlite3* DB) {
    query = "SELECT * FROM CATEGORY;";

    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

void DBManager_Category::updateDataCategory(sqlite3* DB, char* messageError, string sql, int exit, string id, string type) {
    sql = "UPDATE CATEGORY SET TYPE = '"+ type +"' WHERE CATEGORY_ID = '"+ id +"';";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) 
    {
        cerr << "\nError update!\n";
        sqlite3_free(messageError);
    }
    else
        cout << "\nUpdate data successfully!\n";
}

void DBManager_Category::deleteDataCategory(sqlite3* DB, char* messageError, string sql, int exit, string id) {
    sql = "DELETE FROM CATEGORY WHERE CATEGORY_ID = '"+ id +"';";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) 
    {
        cerr << "\nError delete!\n";
        sqlite3_free(messageError);
    }
    else
        cout << "\nDelete data successfully!\n";
}

string DBManager_Category::getCategoryNameBasedOnId(sqlite3* DB, string id) {
    string storedtype;

    query = "SELECT * FROM CATEGORY WHERE CATEGORY_ID = '"+ id +"';";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW)
    {
        storedtype = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));;
    }

    sqlite3_finalize(stmt);

    return storedtype;
}

bool DBManager_Category::checkIfCategoryUnique(sqlite3* DB, string type) {
    query = "SELECT TYPE FROM CATEGORY WHERE TYPE = '"+ type +"';";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW) 
    {
        //Type found
        sqlite3_finalize(stmt);
        return false;
    } 
    else if (exit == SQLITE_DONE) 
    {
        //Type not found
        sqlite3_finalize(stmt);
        return true;
    }
    else
    {
        sqlite3_finalize(stmt);
        return false;
    }
}

bool DBManager_Category::checkIfAnyCategoryExists(sqlite3* DB) {
    query = "SELECT * FROM CATEGORY;";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW) 
    {
        //Category found
        sqlite3_finalize(stmt);
        return true;
    } 
    else if (exit == SQLITE_DONE) 
    {
        //Category not found
        sqlite3_finalize(stmt);
        return false;
    }
    else
    {
        sqlite3_finalize(stmt);
        return false;
    }
}

bool DBManager_Category::checkIfCategoryExistsBasedOnId(sqlite3* DB, string id) {
    query = "SELECT * FROM CATEGORY WHERE CATEGORY_ID = '"+ id +"';";

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