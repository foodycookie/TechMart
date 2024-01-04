#pragma once

#include<string>
using std::string;

class Product {
    public:
    int id;
    string type;
    string name;
    double price;
    int quantity;

    Product(int id, string type, string name, double price, int quantity);
};