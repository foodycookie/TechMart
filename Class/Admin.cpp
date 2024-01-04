#include<iostream>
#include "Admin.h"
using std::cout;

Admin::Admin(int id, string username, string password): id(id), username(username),password(password) {}

void Admin::checkInformation(Admin admin) {
    cout << "\nID: " << admin.id << "\nUsername: " << admin.username << "\nPassword: " << admin.password << "\n";
}
