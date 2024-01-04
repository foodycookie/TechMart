#pragma once

#include<iostream>
#include<string>
using std::string;

class Customer {
    public:
    int id;
    string username;
    string password;
    double cash;

    Customer(int id, string username, string password, double cash);
    
    void checkInformation(Customer customer);
};