#include<iostream>
#include<string>
using std::cout;
using std::string;
#include "Customer.h"

Customer::Customer(int id, string username, string password, double cash): id(id),username(username),password(password),cash(cash) {}

void Customer::checkInformation(Customer customer) {
    cout << "\nID: " << customer.id << "\nUsername: " << customer.username << "\nPassword: " << customer.password << "\nCash: " << customer.cash << "\n";
}