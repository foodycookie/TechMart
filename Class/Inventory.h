#pragma once

#include<string>
using std::string;
#include "Product.h"

class Inventory {
    public:
    Product product;
    int quantity;

    Inventory(Product product, int quantity);
};