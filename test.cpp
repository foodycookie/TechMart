#include<iostream>
#include<string>
using namespace std;

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
            cout << id << endl << type << endl << name << endl << brand << endl << price;
        }
};

int main() {
    keyboard keyboard1("MXMECHANICAL","Logitech",979,"keyboard",1);
    keyboard1.getInformation();
}
