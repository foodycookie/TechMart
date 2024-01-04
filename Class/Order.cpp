#include<string>
using std::string;
#include "Order.h"

Order::Order(int orderId, int customerId, int productId, int quantity, double totalPrice): orderId(orderId), customerId(customerId), productId(productId), quantity(quantity), totalPrice(totalPrice) {}