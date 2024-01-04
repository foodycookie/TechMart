#pragma once

#include<string>
using std::string;
#include "Product.h"
#include "Customer.h"

class Order {
    public:
    int orderId;
    int customerId;
    int productId;
    int quantity;
    double totalPrice;

    Order(int orderId, int customerId, int productId, int quantity, double totalPrice);
};