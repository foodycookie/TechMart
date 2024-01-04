#include "DBManager_Order.h"
#include<iostream>
using std::cout;
using std::cerr;

void DBManager_Order::insertDataOrder(sqlite3* DB, char* messageError, string sql, int exit, string customerID, string productID, string quantity, string totalPrice) {
    sql = "INSERT INTO \"ORDER\"(CUSTOMER_ID, PRODUCT_ID, QUANTITY, TOTAL_PRICE) VALUES('"+ customerID +"', '"+ productID +"', '"+ quantity +"', '"+ totalPrice +"');";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) 
    {
        cerr << "\nError insert!\n";
        sqlite3_free(messageError);
    }
    else
        cout << "\nInsert order successful!\n";
}

void DBManager_Order::getAllOrder(sqlite3* DB) {
    query = "SELECT * FROM \"ORDER\";";

    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

void DBManager_Order::updateDataOrder(sqlite3* DB, char* messageError, string sql, int exit, string id, string customerID, string productID, string quantity, string totalPrice) {
    sql = "UPDATE \"ORDER\" SET CUSTOMER_ID = '"+ customerID +"', PRODUCT_ID = '"+ productID +"', QUANTITY = '"+ quantity +"', TOTAL_PRICE = '"+ totalPrice +"' WHERE ORDER_ID = '"+ id +"';";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) 
    {
        cerr << "\nError update!\n";
        sqlite3_free(messageError);
    }
    else
        cout << "\nUpdate data successfully!\n";
}

void DBManager_Order::deleteDataOrder(sqlite3* DB, char* messageError, string sql, int exit, string id) {
    sql = "DELETE FROM \"ORDER\" WHERE ORDER_ID = '"+ id +"';";

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) 
    {
        cerr << "\nError delete!\n";
        sqlite3_free(messageError);
    }
    else
        cout << "\nOrder delete successfully!\n";
}

void DBManager_Order::getAllOrderBasedOnCustomer(sqlite3* DB, string id) {
    query = "SELECT * FROM \"ORDER\" WHERE CUSTOMER_ID = '"+ id +"';";

    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

Order DBManager_Order::getOrderDataBasedOnId(sqlite3* DB, string id) {
    int storedOrderId, storedCustomerId, storedProductId, storedQuantity;
    double storedTotalPrice;

    query = "SELECT * FROM \"ORDER\" WHERE ORDER_ID = '"+ id +"';";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW)
    {
        storedOrderId = sqlite3_column_int(stmt, 0);
        storedCustomerId = sqlite3_column_int(stmt, 1);;
        storedProductId = sqlite3_column_int(stmt, 2);;
        storedQuantity = sqlite3_column_int(stmt, 3);
        storedTotalPrice = sqlite3_column_double(stmt, 4);
    }

    Order order = Order(storedOrderId, storedCustomerId, storedProductId, storedQuantity, storedTotalPrice);

    sqlite3_finalize(stmt);
    
    return order;
}

void DBManager_Order::getMostPopularItem(sqlite3* DB) {
    query = "SELECT PRODUCT.NAME, SUM(\"ORDER\".QUANTITY) AS TOTAL_SOLD "
    "FROM \"ORDER\" "
    "JOIN PRODUCT ON PRODUCT.PRODUCT_ID = \"ORDER\".PRODUCT_ID "
    "GROUP BY \"ORDER\".PRODUCT_ID "
    "ORDER BY TOTAL_SOLD DESC;";

    sqlite3_exec(DB, query.c_str(), callback, NULL, NULL);
}

bool DBManager_Order::checkIfAnyOrderExistsBasedOnId(sqlite3* DB, string id) {
    query = "SELECT * FROM \"ORDER\" WHERE ORDER_ID = '"+ id +"';";

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

bool DBManager_Order::checkIfAnyOrderExistsBasedOnIdAndCustomerId(sqlite3* DB, string id, string customerId) {
    query = "SELECT * FROM \"ORDER\" WHERE ORDER_ID = '"+ id +"' AND CUSTOMER_ID = '"+ customerId + "';";

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

bool DBManager_Order::checkIfAnyOrderExistsBasedOnCustomerId(sqlite3* DB, string customerId) {
    query = "SELECT * FROM \"ORDER\" WHERE CUSTOMER_ID = '"+ customerId + "';";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW) 
    {
        //found
        sqlite3_finalize(stmt);
        return true;
    } 
    else if (exit == SQLITE_DONE) 
    {
        //not found
        sqlite3_finalize(stmt);
        return false;
    }
    else
    {
        sqlite3_finalize(stmt);
        return false;
    }
}

bool DBManager_Order::checkIfAnyOrderExists(sqlite3* DB) {
    query = "SELECT * FROM \"ORDER\";";

    exit = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, nullptr);

    exit = sqlite3_step(stmt);

    if (exit == SQLITE_ROW) 
    {
        //found
        sqlite3_finalize(stmt);
        return true;
    } 
    else if (exit == SQLITE_DONE) 
    {
        //found
        sqlite3_finalize(stmt);
        return false;
    }
    else
    {
        sqlite3_finalize(stmt);
        return false;
    }
}