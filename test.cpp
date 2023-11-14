#include<iostream>
#include<string>
#include<fstream>
using namespace std;
//all table->category->inventory
class inventory {
    public:
        int id;

        inventory(int id): id(id) {}
};

class category: public inventory {
    public:
        string type;

        category(string type,int id): inventory(id),type(type) {}
};

class keyboard: public category {
    public:
        string name;
        string brand;
        double price;


        keyboard(string name,string brand,double price,string type,int id): category(type,id),name(name),brand(brand),price(price) {}

        void getInformation() {
            cout << id << '\n' << type << '\n' << name << '\n' << brand << '\n' << price;
        }
};

class user {
    public:
        string username;
        string password;

        user(string username="",string password=""): username(username),password(password) {}
};

class admin {
    public:
        string username;
        string password;

        admin(string username="",string password=""): username(username),password(password) {}
};



int main() { 
    bool login = true;
    bool verify = false;
    int choice;
    string loginusername;
    string loginpassword;

    user user1;

    fstream userfile;
    userfile.open("userfile",ios::app);

    while(login) {
        cout << "1. Login\n2. Sign Up\n3. Exit\n" << '\n';
        cin >> choice;
        cout << '\n';

        if(choice==1) {
            cout << "Enter your username: ";
            cin >> loginusername;
            cout << "Enter your password: ";
            cin >> loginpassword;
            cout << '\n';

            if(loginusername==user1.username&&loginpassword==user1.password) {
                cout << "Login Successful!\n" << '\n';
            }
            else {
                cout << "User not found!\n" << '\n';
            }
        }
        else if(choice==2) {
            cout << "Enter your username: ";
            cin >> user1.username;
            cout << "Enter your password: ";
            cin >> user1.password;
            cout << '\n';
            userfile << user1.username + " " + user1.password + '\n';
            userfile.close();
        }
        else if(choice==3) {
            cout << "Thanks for your visit.";
            exit(0);
        }
        else {
            cout << "Please enter the right number." << '\n';
        }
    }
}
