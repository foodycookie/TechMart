#include<string>
using std::string;
#include "Product.h"

Product::Product(int id, string type, string name, double price, int quantity): id(id),type(type),name(name),price(price),quantity(quantity) {}