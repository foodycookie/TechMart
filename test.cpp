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
};

int main() {
    keyboard keyboard1("MXMECHANICAL","Logitech",979,"keyboard",1);

    cout << keyboard1.id << endl << keyboard1.type << endl << keyboard1.name << endl << keyboard1.brand << endl << keyboard1.price;
}