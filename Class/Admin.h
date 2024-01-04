#pragma once

#include<iostream>
#include<string>
using std::string;

class Admin {
    public:
    int id;
    string username;
    string password;

    Admin(int id, string username, string password);

    void checkInformation(Admin admin);
};