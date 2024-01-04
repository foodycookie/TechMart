//decimal error

//handle input number error and turn swtich case to if else if need

//seperate menu into smaller file adminMenu()

//on delete cascade error

#include<iostream>
#include<string>
#include "Sqlite3/sqlite3.h"
using std::cout;
using std::cin;
using std::string;
using std::cerr;
using std::to_string;
#include "Class/Customer.h"
#include "Class/Admin.h"
#include "Class/Supplier.h"
#include "Class/Category.h"
#include "Class/Product.h"
#include "Class/Inventory.h"
#include "Class/Order.h"
#include "DBManager/DBManager.h"
#include "DBManager/DBManager_Admin.h"
#include "DBManager/DBManager_Customer.h"
#include "DBManager/DBManager_Supplier.h"
#include "DBManager/DBManager_Category.h"
#include "DBManager/DBManager_Product.h"
#include "DBManager/DBManager_Inventory.h"
#include "DBManager/DBManager_Order.h"

struct ProductNode {
    public:
    int id;
    string type;
    string name;
    double price;
    int quantity;
    struct ProductNode* next;

    ProductNode(int id, string type, string name, double price, int quantity) : id(id), type(type), name(name), price(price), quantity(quantity), next(nullptr) {}    
};

class ProductList {
    public:
    ProductNode* head = NULL;

    void addProduct(int id, string type, string name, double price, int quantity) {
        ProductNode* newProduct = new ProductNode(id, type, name, price, quantity);

        if (head == nullptr) 
        {
            head = newProduct;
        } 
        
        else 
        {
            ProductNode* tempNode = head;

            while (tempNode->next != nullptr) 
            {
                tempNode = tempNode->next;
            }

            tempNode->next = newProduct;
        }
    }

    void addEveryProductInVector(vector<Product> products) {
        for (Product product : products) 
        {
            addProduct(product.id, product.type, product.name, product.price, product.quantity);
        }
    }

    void printList() {
        ProductNode* tempNode = head;

        cout << "No" << "  " << "Category" << "  " << "Name" << "  " << "Price" << "  " << "Quantity\n";
        while (tempNode != nullptr) {
            cout << tempNode->id << "   ";
            cout << tempNode->type << "   ";
            cout << tempNode->name << "   ";
            cout << tempNode->price << "   ";
            cout << tempNode->quantity << "   ";
            cout << "\n";
            tempNode = tempNode->next;
        }
    }
};

class Shop {
    public:
    Customer currentCustomer = Customer(0, "", "", 0);
    Admin currentAdmin = Admin(0, "", "");
    
    DBManager dbManager;
    DBManager_Customer dbManagerCustomer;
    DBManager_Admin dbManagerAdmin;
    DBManager_Supplier dbManagerSupplier;
    DBManager_Category dbManagerCategory;
    DBManager_Inventory dbManagerInventory;
    DBManager_Product dbManagerProduct;
    DBManager_Order dbManagerOrder;

    void defaultMenu() {
        string selection;

        do
        {
            cout << "\nWelcome to TechMart\n\n" << "What do you want to do?\n" << "0 - Login\n" << "1 - Register\n" << "2 - Exit\n\n";
            cin >> selection;

            cout << "\n";

            if (selection == "0")
            {
                loginMenu();
                break;
            }

            else if (selection == "1")
            {
                registerMenu();
                break;
            }

            else if (selection == "2")
            {
                cout << "Thank You!\n";
                exit(0);
            }
            
            else
            {
                cout << "Error!\n";
            }
        } while (true);
    }

    void loginMenu() {
        string selection;
        string username,password;
        bool loginValid;

        dbManager.openDB();

        cout << "Enter your username: \n";
        cin >> username;
        cout << "Enter your password: \n";
        cin >> password;

        loginValid = dbManagerCustomer.checkIfCustomerLoginValid(dbManager.DB, username, password);

        if (loginValid)
        {
            cout << "\nLogin as customer!\n";

            currentCustomer = Customer(dbManagerCustomer.getCustomerDataBasedOnUsername(dbManager.DB, username));

            customerMenu();
        }

        loginValid = dbManagerAdmin.checkIfAdminLoginValid(dbManager.DB, username, password);

        if (loginValid)
        {
            cout << "\nLogin as admin!\n";

            currentAdmin = Admin(dbManagerAdmin.getAdminDataBasedOnUsername(dbManager.DB, username));

            adminMenu();
        }
        
        cout << "\nLogin failed!\n";

        dbManager.closeDB();

        defaultMenu();
    }

    void registerMenu() {
        string selection;
        string username,password;
        bool uniqueUsername;

        dbManager.openDB();

        do
        {
            cout << "Enter your username: \n";
            cin >> username;

            uniqueUsername = dbManager.checkIfUsernameUnique(dbManager.DB, username);

            if (!uniqueUsername)
            {
                cout << "\nUsername was taken!\n\n";
            }
        } while (!uniqueUsername);
        
        cout << "Enter your password: \n";
        cin >> password;
        cout << "\n";

        cout << "Register as?\n" << "0 - Customer\n" << "1 - Admin\n\n";
        cin >> selection;

        if (selection == "0")
        {
            dbManagerCustomer.insertDataCustomer(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, username, password, "0");
        }
        

        if (selection == "1")
        {
            dbManagerAdmin.insertDataAdmin(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, username, password);
        }        

        dbManager.closeDB();

        defaultMenu();
    }

    void customerMenu() {
        string selection, id;
        int quantity, oldQuantity;
        double topUpAmount, totalPrice;
        bool existsOrNot;
        Product chosenProduct = Product(0,"","",0,0);
        Order chosenOrder = Order(0, 0, 0, 0, 0);
 
        do
        {
            cout << "\nWelcome Customer " << currentCustomer.username << "!\n" << "What do you want to do?\n" << "0 - Buy Stuff\n" << "1 - Top Up\n" << "2 - Check Order\n" << "3 - Delete Order and Refund\n" << "4 - Check information\n" << "5 - Logout\n" << "\n";
            cin >> selection;

            if (selection == "0")
            {
                dbManager.openDB();

                dbManagerInventory.listOutProductIDCategoryNamePriceQuantity(dbManager.DB);

                existsOrNot = dbManagerInventory.checkIfAnyBuyableExists(dbManager.DB);

                if(!existsOrNot) 
                {
                    cout << "\nThere is no stock!\n";

                    customerMenu();
                }

                do
                {
                    cout << "Enter the ID of the item you wish to buy: ";
                    cin >> id;
                    cout << "\n";

                    existsOrNot = dbManagerProduct.checkIfAnyProductExistsBasedOnId(dbManager.DB, id);

                    if (!existsOrNot)
                    {
                        cout << "Enter exists ID!\n";
                    }
                } while (!existsOrNot);  

                chosenProduct = dbManagerInventory.getProductIDCategoryNamePriceQuantity(dbManager.DB, id);

                cout << "Enter the quantity: ";
                cin >> quantity;
                cout << "\n";

                if (quantity>=chosenProduct.quantity)
                {
                    cout << "\nExceed product's current quantity!\n";
                    continue;
                }

                totalPrice = chosenProduct.price*quantity;

                cout << "Total price: " << totalPrice << "\n";

                if (currentCustomer.cash<totalPrice)
                {
                    cout << "\n\nYou do not have enough money!\n";
                }
                else
                {
                    dbManagerOrder.insertDataOrder(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(currentCustomer.id), to_string(chosenProduct.id), to_string(quantity), to_string(totalPrice));

                    currentCustomer.cash-=totalPrice;

                    dbManagerCustomer.updateDataCustomer(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(currentCustomer.id), currentCustomer.username, currentCustomer.password, to_string(currentCustomer.cash));

                    chosenProduct.quantity -= quantity;

                    dbManagerInventory.updateDataInventory(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(chosenProduct.id), to_string(chosenProduct.quantity));

                    cout << "\nPurchase successful!\n";
                }

                dbManager.closeDB();
            }

            else if (selection == "1")
            {
                dbManager.openDB();

                cout << "Enter the amount: \n";
                cin >> topUpAmount;

                currentCustomer.cash += topUpAmount;

                dbManagerCustomer.updateDataCustomer(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(currentCustomer.id), currentCustomer.username, currentCustomer.password, to_string(currentCustomer.cash));        

                dbManager.closeDB();
            }

            else if (selection == "2")
            {
                cout << "\n";
                dbManager.openDB();

                dbManagerOrder.getAllOrderBasedOnCustomer(dbManager.DB, to_string(currentCustomer.id));

                existsOrNot = dbManagerOrder.checkIfAnyOrderExistsBasedOnCustomerId(dbManager.DB, to_string(currentCustomer.id));

                if(!existsOrNot) 
                {
                    cout << "\nThere is no order!\n";

                    customerMenu();
                }

                dbManager.closeDB();
            }

            else if (selection == "3")
            {
                dbManager.openDB();

                dbManagerOrder.getAllOrderBasedOnCustomer(dbManager.DB, to_string(currentCustomer.id));

                existsOrNot = dbManagerOrder.checkIfAnyOrderExistsBasedOnCustomerId(dbManager.DB, to_string(currentCustomer.id));

                if(!existsOrNot) 
                {
                    cout << "\nThere is no order!\n";

                    customerMenu();
                }

                do
                {
                    cout << "Enter the id you want to delete and refund: ";
                    cin >> id;
                    cout << "\n";

                    existsOrNot = dbManagerOrder.checkIfAnyOrderExistsBasedOnIdAndCustomerId(dbManager.DB, id, to_string(currentCustomer.id));

                    if (!existsOrNot)
                    {
                        cout << "Enter exists ID!\n";
                    }
                } while (!existsOrNot);

                chosenOrder = dbManagerOrder.getOrderDataBasedOnId(dbManager.DB, id);

                currentCustomer.cash += chosenOrder.totalPrice;

                dbManagerCustomer.updateDataCustomer(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(currentCustomer.id), currentCustomer.username, currentCustomer.password, to_string(currentCustomer.cash));   

                oldQuantity = dbManagerInventory.getInventoryQuantityBasedOnProductId(dbManager.DB, to_string(chosenOrder.productId));  

                dbManagerInventory.updateDataInventory(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(chosenOrder.productId), to_string(oldQuantity += chosenOrder.quantity));

                dbManagerOrder.deleteDataOrder(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, id);

                dbManager.closeDB();

                cout << "Refund succesful!\n";
            }

            else if (selection == "4")
            {
                currentCustomer.checkInformation(currentCustomer);
            }

            else if (selection == "5")
            {
                defaultMenu();
                break;
            }

            else
            {
                cout << "Error!\n";
            }
        } while (true);
    }

    void adminMenu() {
        int selection = 0;
        bool uniqueUsername;
        bool existsOrNot;

        int id, newCash, supplierID, categoryID , quantity, oldQuantity;
        double price, totalPrice;
        string newUsername, newPassword, oldUsername;

        vector<Product> listOfProduct;

        ProductList productList;

        Customer customer = Customer(0, "", "", 0);
        Product chosenProduct = Product(0, "", "", 0, 0);
        Order chosenOrder = Order(0, 0, 0, 0, 0);

        do
        {
            cout << "\nWelcome Admin " << currentAdmin.username << "!\n" << "What do you want to do?\n" << "0 - Edit supplier table\n" << "1 - Edit category table\n" << "2 - Edit product table\n" << "3 - Edit inventory table\n" << "4 - Edit order table\n" << "5 - Edit customer table\n" << "6 - Edit admin table\n" << "7 - Get all product\n" << "8 - Get the most popular item\n" << "9 - Check information\n" << "10 - Logout\n" << "\n";
            cin >> selection;          

            switch (selection)
            {
            case 0:
                do
                {
                    cout << "\nWhat do you want to do?\n" << "0 - Get all supplier data\n" << "1 - Create new supplier\n" << "2 - Update supplier data\n" << "3 - Delete supplier\n" << "4 - Back\n" << "\n";
                    cin >> selection;

                    switch (selection)
                    {
                    case 0:
                        dbManager.openDB();

                        dbManagerSupplier.getAllSupplierData(dbManager.DB);

                        existsOrNot = dbManagerSupplier.checkIfAnySupplierExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no supplier!\n";

                            adminMenu();
                        }

                        dbManager.closeDB();

                        break;

                    case 1:
                        dbManager.openDB();

                        do
                        {
                            //Im using the same variable since i dont want to make a new one.
                            //newUsername is supplier's name here
                            //newPassword is supplier's contact info
                            cout << "\nEnter the name: ";
                            cin >> newUsername;

                            uniqueUsername = dbManagerSupplier.checkIfSupplierUnique(dbManager.DB, newUsername);

                            if (!uniqueUsername)
                            {
                                cout << "\nName was taken!\n";
                            }
                        } while (!uniqueUsername);

                        cout << "Enter the contact info: ";
                        cin >> newPassword;

                        dbManagerSupplier.insertDataSupplier(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, newUsername, newPassword);

                        dbManager.closeDB();

                        break;

                    case 2:
                        //Im using the same variable since i dont want to make a new one.
                        //oldUsername is supplier's name before update
                        //newUsername is supplier's name after update
                        //newPassword is supplier's contact info after update
                        dbManager.openDB();

                        existsOrNot = dbManagerSupplier.checkIfAnySupplierExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no supplier!\n";

                            adminMenu();
                        }

                        dbManagerSupplier.getAllSupplierData(dbManager.DB);

                        do
                        {
                            cout << "\nEnter the id you want to update: ";
                            cin >> id;  
                            cout << "\n";

                            existsOrNot = dbManagerSupplier.checkIfSupplierExistsBasedOnId(dbManager.DB, to_string(id));

                            if (!existsOrNot)
                            {
                                cout << "Enter exists ID!\n";
                            }
                        } while (!existsOrNot);

                        oldUsername = dbManagerSupplier.getSupplierNamwBasedOnId(dbManager.DB, to_string(id));

                        do
                        {
                            cout << "Enter the new name: ";
                            cin >> newUsername;

                            if(newUsername!=oldUsername) 
                            {

                                uniqueUsername = dbManagerSupplier.checkIfSupplierUnique(dbManager.DB, newUsername);

                                if (!uniqueUsername)
                                {
                                    cout << "\nName was taken!\n";
                                }
                            }
                            else
                            {
                                break;
                            }
                            
                        } while (!uniqueUsername);

                        
                        cout << "Enter the new contact info: ";
                        cin >> newPassword;

                        dbManagerSupplier.updateDataSupplier(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(id), newUsername, newPassword);

                        dbManager.closeDB();

                        break;

                    case 3:
                        dbManager.openDB();
                        
                        existsOrNot = dbManagerSupplier.checkIfAnySupplierExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no supplier!\n";

                            adminMenu();
                        }

                        dbManagerSupplier.getAllSupplierData(dbManager.DB);

                        do
                        {
                            cout << "\nEnter the id you want to delete: \n";
                            cin >> id;
                            cout << "\n";

                            existsOrNot = dbManagerSupplier.checkIfSupplierExistsBasedOnId(dbManager.DB, to_string(id));

                            if (!existsOrNot)
                            {
                                cout << "Enter exists ID!\n";
                            }
                        } while (!existsOrNot);

                        dbManagerSupplier.deleteDataSupplier(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(id));

                        dbManager.closeDB();

                        break;

                    case 4:
                        adminMenu();
                        break;
                    
                    default:
                        cout << "Error!\n";
                        break;
                    }
                } while (true);
                
                break;

            case 1:
                do
                {
                    cout << "\nWhat do you want to do?\n" << "0 - Get all category data\n" << "1 - Create new category\n" << "2 - Update category data\n" << "3 - Delete category\n" << "4 - Back\n" << "\n";
                    cin >> selection;

                    switch (selection)
                    {
                    case 0:
                        dbManager.openDB();

                        dbManagerCategory.getAllCategoryData(dbManager.DB);

                        existsOrNot = dbManagerCategory.checkIfAnyCategoryExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no category!\n";

                            adminMenu();
                        }

                        dbManager.closeDB();
                        break;

                    case 1:
                        dbManager.openDB();

                        do
                        {
                            //Im using the same variable since i dont want to make a new one.
                            //newUsername is category's type
                            cout << "\nEnter the type: ";
                            cin >> newUsername;

                            uniqueUsername = dbManagerCategory.checkIfCategoryUnique(dbManager.DB, newUsername);

                            if (!uniqueUsername)
                            {
                                cout << "\nType already exists!\n";
                            }
                        } while (!uniqueUsername);

                        dbManagerCategory.insertDataCategory(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, newUsername);

                        dbManager.closeDB();

                        break;

                    case 2:
                        //Im using the same variable since i dont want to make a new one.
                        //oldUsername is category's type before update
                        //newUsername is cateogry's type after update
                        dbManager.openDB();

                        existsOrNot = dbManagerCategory.checkIfAnyCategoryExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no category!\n";

                            adminMenu();
                        }

                        dbManagerCategory.getAllCategoryData(dbManager.DB);

                        do
                        {
                            cout << "\nEnter the id you want to delete: \n";
                            cin >> id;
                            cout << "\n";

                            existsOrNot = dbManagerCategory.checkIfCategoryExistsBasedOnId(dbManager.DB, to_string(id));

                            if (!existsOrNot)
                            {
                                cout << "Enter exists ID!\n";
                            }
                        } while (!existsOrNot);

                        cout << "\nEnter the id you want to update: ";
                        cin >> id;

                        oldUsername = dbManagerCategory.getCategoryNameBasedOnId(dbManager.DB, to_string(id));

                        do
                        {
                            cout << "Enter the new name: ";
                            cin >> newUsername;

                            if(newUsername!=oldUsername) 
                            {

                                uniqueUsername = dbManagerCategory.checkIfCategoryUnique(dbManager.DB, newUsername);

                                if (!uniqueUsername)
                                {
                                    cout << "\nName was taken!\n";
                                }
                            }
                            else
                            {
                                break;
                            }
                            
                        } while (!uniqueUsername);

                        dbManagerCategory.updateDataCategory(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(id), newUsername);

                        dbManager.closeDB();

                        break;

                    case 3:
                        dbManager.openDB();

                        existsOrNot = dbManagerCategory.checkIfAnyCategoryExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no category!\n";

                            adminMenu();
                        }

                        dbManagerCategory.getAllCategoryData(dbManager.DB);

                        do
                        {
                            cout << "\nEnter the id you want to delete: \n";
                        cin >> id;
                            cout << "\n";

                            existsOrNot = dbManagerCategory.checkIfCategoryExistsBasedOnId(dbManager.DB, to_string(id));

                            if (!existsOrNot)
                            {
                                cout << "Enter exists ID!\n";
                            }
                        } while (!existsOrNot);

                        dbManagerCategory.deleteDataCategory(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(id));

                        dbManager.closeDB();

                        break;

                    case 4:
                        adminMenu();
                        break;
                    
                    default:
                        cout << "Error!\n";
                        break;
                    }
                } while (true);

                break;

            case 2:
                do
                {
                    cout << "\nWhat do you want to do?\n" << "0 - Get all product data\n" << "1 - Create new product\n" << "2 - Update product data\n" << "3 - Delete product\n" << "4 - Back\n" << "\n";
                    cin >> selection;

                    switch (selection)
                    {
                    case 0:
                        dbManager.openDB();

                        cout << "\n";

                        dbManagerProduct.getAllProductData(dbManager.DB);

                        existsOrNot = dbManagerProduct.checkIfAnyProductExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no product!\n";

                            adminMenu();
                        }

                        dbManager.closeDB();
                        break;

                    case 1:
                        dbManager.openDB();

                        existsOrNot = dbManagerSupplier.checkIfAnySupplierExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no supplier!\n";

                            adminMenu();
                        }
                        else
                        {
                            cout << "\n";

                            dbManagerSupplier.getAllSupplierData(dbManager.DB);

                            do
                            {
                                cout << "\nEnter the supplier's id: ";
                                cin >> supplierID;
                                cout << "\n";

                                existsOrNot = dbManagerSupplier.checkIfSupplierExistsBasedOnId(dbManager.DB, to_string(supplierID));

                                if (!existsOrNot)
                                {
                                    cout << "Enter exists ID!\n";
                            }
                            } while (!existsOrNot);                            
                        }

                        existsOrNot = dbManagerCategory.checkIfAnyCategoryExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no category!\n";

                            adminMenu();
                        }
                        else
                        {
                            dbManagerCategory.getAllCategoryData(dbManager.DB);

                            do
                            {
                                cout << "\nEnter the category's id: ";
                                cin >> categoryID;
                                cout << "\n";

                            existsOrNot = dbManagerCategory.checkIfCategoryExistsBasedOnId(dbManager.DB, to_string(categoryID));

                            if (!existsOrNot)
                            {
                                cout << "Enter exists ID!\n";
                            }
                            } while (!existsOrNot);         
                        }
                        
                        do
                        {
                            //Im using the same variable since i dont want to make a new one.
                            //newUsername is product's name
                            cout << "Enter the name: ";
                            cin >> newUsername;
                            cout << "\n";

                            uniqueUsername = dbManagerProduct.checkIfProductUnique(dbManager.DB, newUsername);

                            if (!uniqueUsername)
                            {
                                cout << "\nName already exists!\n";
                            }
                        } while (!uniqueUsername);

                        cout << "Enter the price: ";
                        cin >> price;

                        dbManagerProduct.insertDataProduct(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(supplierID), to_string(categoryID), newUsername, to_string(price));

                        dbManager.closeDB();

                        break;

                    case 2:
                        //Im using the same variable since i dont want to make a new one.
                        //oldUsername is product's name before update
                        //newUsername is product's name after update
                        dbManager.openDB();

                        existsOrNot = dbManagerProduct.checkIfAnyProductExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no product!\n";

                            adminMenu();
                        }

                        dbManagerProduct.getAllProductData(dbManager.DB);

                        do
                        {
                            cout << "\nEnter the id you want to update: ";
                            cin >> id;
                            cout << "\n";

                            existsOrNot = dbManagerProduct.checkIfAnyProductExistsBasedOnId(dbManager.DB, to_string(id));

                            if (!existsOrNot)
                            {
                                cout << "Enter exists ID!\n";
                            }
                        } while (!existsOrNot);

                        existsOrNot = dbManagerSupplier.checkIfAnySupplierExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no supplier!\n";

                            adminMenu();
                        }
                        else
                        {
                            dbManagerSupplier.getAllSupplierData(dbManager.DB);

                            do
                            {
                                cout << "Enter the new supplier's id: \n";
                            cin >> supplierID;

                            existsOrNot = dbManagerSupplier.checkIfSupplierExistsBasedOnId(dbManager.DB, to_string(supplierID));

                            if (!existsOrNot)
                            {
                                cout << "Enter exists ID!\n";
                            }
                            } while (!existsOrNot);                            
                        }

                        existsOrNot = dbManagerCategory.checkIfAnyCategoryExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no category!\n";

                            adminMenu();
                        }
                        else
                        {
                            dbManagerCategory.getAllCategoryData(dbManager.DB);

                            do
                            {
                                cout << "Enter the new category's id: \n";
                            cin >> categoryID;

                            existsOrNot = dbManagerCategory.checkIfCategoryExistsBasedOnId(dbManager.DB, to_string(categoryID));

                            if (!existsOrNot)
                            {
                                cout << "Enter exists ID!\n";
                            }
                            } while (!existsOrNot);         
                        }

                        oldUsername = dbManagerProduct.getProductNameBasedOnId(dbManager.DB, to_string(id));

                        do
                        {
                            cout << "Enter the new name: ";
                            cin >> newUsername;

                            if(newUsername!=oldUsername) 
                            {
                                uniqueUsername = dbManagerProduct.checkIfProductUnique(dbManager.DB, newUsername);

                                if (!uniqueUsername)
                                {
                                    cout << "\nName was taken!\n";
                                }
                            }
                            else
                            {
                                break;
                            }
                        } while (!uniqueUsername);

                        cout << "Enter new price: ";
                        cin >> price;

                        dbManagerProduct.updateDataProduct(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(id), to_string(supplierID) ,to_string(categoryID), newUsername, to_string(price));

                        dbManager.closeDB();

                        break;

                    case 3:
                        dbManager.openDB();

                        existsOrNot = dbManagerProduct.checkIfAnyProductExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no product!\n";

                            adminMenu();
                        }

                        dbManagerProduct.getAllProductData(dbManager.DB);

                        do
                        {
                            cout << "\nEnter the id you want to delete: \n";
                            cin >> id;
                            cout << "\n";

                            existsOrNot = dbManagerProduct.checkIfAnyProductExistsBasedOnId(dbManager.DB, to_string(id));

                            if (!existsOrNot)
                            {
                                cout << "Enter exists ID!\n";
                            }
                        } while (!existsOrNot);

                        dbManagerProduct.deleteDataProduct(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(id));

                        dbManager.closeDB();

                        break;

                    case 4:
                        adminMenu();
                        break;
                    
                    default:
                        cout << "Error!\n";
                        break;
                    }
                } while (true);

                break;

            case 3:
                do
                {
                    cout << "\nWhat do you want to do?\n" << "0 - Get inventory data\n" << "1 - Update item data\n" << "2 - Back\n" << "\n";
                    cin >> selection;

                    switch (selection)
                    {
                    case 0:
                        dbManager.openDB();

                        cout << "\n";

                        dbManagerInventory.getAllInventoryData(dbManager.DB);

                        existsOrNot = dbManagerProduct.checkIfAnyProductExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no product!\n";

                            adminMenu();
                        }

                        dbManager.closeDB();
                        break;

                    case 1:
                        dbManager.openDB();

                        existsOrNot = dbManagerProduct.checkIfAnyProductExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no product!\n";

                            adminMenu();
                        }

                        dbManagerInventory.getAllInventoryData(dbManager.DB);

                        do
                        {
                            cout << "\nEnter the id you want to update: ";
                            cin >> id;
                            cout << "\n";

                            existsOrNot = dbManagerInventory.checkIfAnyInventoryExistsBasedOnId(dbManager.DB, to_string(id));

                            if (!existsOrNot)
                            {
                                cout << "Enter exists ID!\n";
                            }
                        } while (!existsOrNot);

                        cout << "Enter new quantity: ";
                        cin >> quantity;

                        dbManagerInventory.updateDataInventory(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(id), to_string(quantity));

                        dbManager.closeDB();

                        break;

                    case 2:
                        adminMenu();
                        break;
                           
                    default:
                        cout << "Error!\n";
                        break;
                    }
                } while (true);
                break;

            case 4:
                do
                {
                    cout << "\nWhat do you want to do?\n" << "0 - Get all order data\n" << "1 - Update order\n" << "2 - Delete order and refund\n" << "3 - Back\n" << "\n";
                    cin >> selection;

                    switch (selection)
                    {
                    case 0:
                        cout << "\n";

                        dbManager.openDB();

                        dbManagerOrder.getAllOrder(dbManager.DB);

                        existsOrNot = dbManagerOrder.checkIfAnyOrderExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no order!\n";

                            adminMenu();
                        }

                        dbManager.closeDB();

                        break;

                    case 1:
                        dbManager.openDB();

                        dbManagerOrder.getAllOrder(dbManager.DB);

                        existsOrNot = dbManagerOrder.checkIfAnyOrderExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no order!\n";

                            adminMenu();
                        }

                        do
                        {
                            cout << "Enter the id you want to update: ";
                            cin >> id;
                            cout << "\n";

                            existsOrNot = dbManagerOrder.checkIfAnyOrderExistsBasedOnId(dbManager.DB, to_string(id));

                            if (!existsOrNot)
                            {
                                cout << "Enter exists ID!\n";
                            }
                        } while (!existsOrNot);  

                        existsOrNot = dbManagerProduct.checkIfAnyProductExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no product!\n";

                            adminMenu();
                        }

                        chosenOrder = dbManagerOrder.getOrderDataBasedOnId(dbManager.DB, to_string(id));

                        customer = dbManagerCustomer.getCustomerDataBasedOnId(dbManager.DB, to_string(chosenOrder.customerId));

                        customer.cash += chosenOrder.totalPrice;

                        dbManagerCustomer.updateDataCustomer(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(currentCustomer.id), currentCustomer.username, currentCustomer.password, to_string(customer.cash));   

                        oldQuantity = dbManagerInventory.getInventoryQuantityBasedOnProductId(dbManager.DB, to_string(chosenOrder.productId));  

                        dbManagerInventory.updateDataInventory(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(chosenOrder.productId), to_string(oldQuantity += chosenOrder.quantity));

                        dbManagerInventory.listOutProductIDCategoryNamePriceQuantity(dbManager.DB);

                        do
                        {
                            cout << "Enter the new product ID: ";
                            cin >> id;
                            cout << "\n";

                            existsOrNot = dbManagerProduct.checkIfAnyProductExistsBasedOnId(dbManager.DB, to_string(id));

                            if (!existsOrNot)
                            {
                                cout << "Enter exists ID!\n";
                            }
                        } while (!existsOrNot);

                        
                        chosenProduct = dbManagerInventory.getProductIDCategoryNamePriceQuantity(dbManager.DB, to_string(id));

                        cout << "Enter the new quantity: ";
                        cin >> quantity;
                        cout << "\n";

                        if (quantity>=chosenProduct.quantity)
                        {
                            cout << "\nExceed product's current quantity!\n";
                            continue;
                        }

                        totalPrice = chosenProduct.price*quantity;

                        cout << "Total price: " << totalPrice << "\n";

                        if (customer.cash < totalPrice)
                        {
                            cout << "\n\nThe customer don't have enough money!\n";
                        }
                        else
                        {
                            dbManagerOrder.updateDataOrder(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(chosenOrder.orderId), to_string(customer.id), to_string(chosenProduct.id), to_string(quantity), to_string(totalPrice));

                            customer.cash -= totalPrice;

                            dbManagerCustomer.updateDataCustomer(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(customer.id), customer.username, customer.password, to_string(customer.cash));

                            chosenProduct.quantity -= quantity;

                            dbManagerInventory.updateDataInventory(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(chosenProduct.id), to_string(chosenProduct.quantity));
                        }

                        dbManager.closeDB();

                        break;

                    case 2:
                        dbManager.openDB();

                        dbManagerOrder.getAllOrder(dbManager.DB);

                        existsOrNot = dbManagerOrder.checkIfAnyOrderExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no order!\n";

                            adminMenu();
                        }
                        
                        do
                        {
                            cout << "Enter the id you want to delete and refund: ";
                            cin >> id;
                            cout << "\n";

                            existsOrNot = dbManagerOrder.checkIfAnyOrderExistsBasedOnId(dbManager.DB, to_string(id));

                            if (!existsOrNot)
                            {
                                cout << "Enter exists ID!\n";
                            }
                        } while (!existsOrNot);  

                        chosenOrder = dbManagerOrder.getOrderDataBasedOnId(dbManager.DB, to_string(id));

                        customer = Customer(dbManagerCustomer.getCustomerDataBasedOnId(dbManager.DB, to_string(chosenOrder.customerId)));

                        customer.cash += chosenOrder.totalPrice;

                        dbManagerCustomer.updateDataCustomer(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(customer.id), customer.username, customer.password, to_string(customer.cash));   

                        oldQuantity = dbManagerInventory.getInventoryQuantityBasedOnProductId(dbManager.DB, to_string(chosenOrder.productId));  

                        dbManagerInventory.updateDataInventory(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(chosenOrder.productId), to_string(oldQuantity += chosenOrder.quantity));

                        dbManagerOrder.deleteDataOrder(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(id));

                        dbManager.closeDB();

                        cout << "Refund succesful!\n";

                        break;

                    case 3:
                        adminMenu();
                        break;
                    
                    default:
                        cout << "Error!\n";
                        break;
                    }
                } while (true);  
            
                break;

            case 5:
                do
                {
                    cout << "\nWhat do you want to do?\n" << "0 - Get all customer data\n" << "1 - Create new customer\n" << "2 - Update customer data\n" << "3 - Delete customer\n" << "4 - Back\n" << "\n";
                    cin >> selection;

                    switch (selection)
                    {
                    case 0:
                        cout << "\n";

                        dbManager.openDB();

                        dbManagerCustomer.getAllCustomerData(dbManager.DB);

                        existsOrNot = dbManagerCustomer.checkIfAnyCustomerExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no customer!\n";
                        }

                        dbManager.closeDB();

                        break;

                    case 1:
                        dbManager.openDB();

                        do
                        {
                            cout << "\nEnter the username: ";
                            cin >> newUsername;


                            uniqueUsername = dbManager.checkIfUsernameUnique(dbManager.DB, newUsername);

                            if (!uniqueUsername)
                            {
                                cout << "\nUsername was taken!\n\n";
                            }
                        } while (!uniqueUsername);

                        cout << "Enter the password: ";
                        cin >> newPassword;
                        cout << "Enter the cash: ";
                        cin >> newCash;

                        dbManagerCustomer.insertDataCustomer(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, newUsername, newPassword, to_string(newCash));

                        dbManager.closeDB();

                        break;

                    case 2:
                        dbManager.openDB();

                        existsOrNot = dbManagerCustomer.checkIfAnyCustomerExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no customer!\n";

                            adminMenu();
                        }

                        dbManagerCustomer.getAllCustomerData(dbManager.DB);

                        do
                        {
                            cout << "\nEnter the id you want to update: ";
                            cin >> id;
                            cout << "\n";

                            existsOrNot = dbManagerCustomer.checkIfAnyCustomerExistsBasedOnId(dbManager.DB, to_string(id));

                            if (!existsOrNot)
                            {
                                cout << "Enter exists ID!\n";
                            }
                        } while (!existsOrNot); 

                        oldUsername = (dbManagerCustomer.getCustomerDataBasedOnId(dbManager.DB, to_string(id))).username;

                        do
                        {
                            cout << "Enter the new username: ";
                            cin >> newUsername;

                            if(newUsername!=oldUsername) 
                            {

                                uniqueUsername = dbManager.checkIfUsernameUnique(dbManager.DB, newUsername);

                                if (!uniqueUsername)
                                {
                                    cout << "\nUsername was taken!\n\n";
                                }
                            }
                            else
                            {
                                break;
                            }
                            
                        } while (!uniqueUsername);

                        
                        cout << "Enter the new password: ";
                        cin >> newPassword;
                        cout << "Enter the cash: ";
                        cin >> newCash;

                        dbManagerCustomer.updateDataCustomer(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(id), newUsername, newPassword, to_string(newCash));

                        dbManager.closeDB();

                        break;

                    case 3:
                        dbManager.openDB();

                        existsOrNot = dbManagerCustomer.checkIfAnyCustomerExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no customer!\n";

                            adminMenu();
                        }

                        dbManagerCustomer.getAllCustomerData(dbManager.DB);

                        do
                        {
                            cout << "\nEnter the id you want to delete: ";
                            cin >> id;
                            cout << "\n";

                            existsOrNot = dbManagerCustomer.checkIfAnyCustomerExistsBasedOnId(dbManager.DB, to_string(id));

                            if (!existsOrNot)
                            {
                                cout << "Enter exists ID!\n";
                            }
                        } while (!existsOrNot); 

                        dbManagerCustomer.deleteDataCustomer(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(id));

                        dbManager.closeDB();

                        break;

                    case 4:
                        adminMenu();
                        break;
                    
                    default:
                        cout << "Error!\n";
                        break;
                    }
                } while (true);  
            
                break;

            case 6:
                do
                {
                    cout << "\nWhat do you want to do?\n" << "0 - Get all admin data\n" << "1 - Create new admin\n" << "2 - Update admin data\n" << "3 - Delete admin\n" << "4 - Back\n" << "\n";
                    cin >> selection;

                    switch (selection)
                    {
                    case 0:
                        cout << "\n";

                        dbManager.openDB();

                        dbManagerAdmin.getAllAdminData(dbManager.DB);

                        existsOrNot = dbManagerAdmin.checkIfAnyAdminExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no admin!\n";
                        }

                        dbManager.closeDB();

                        break;

                    case 1:
                        dbManager.openDB();

                        do
                        {
                            cout << "\nEnter the username: ";
                            cin >> newUsername;

                            uniqueUsername = dbManager.checkIfUsernameUnique(dbManager.DB, newUsername);

                            if (!uniqueUsername)
                            {
                                cout << "\nUsername was taken!\n\n";
                            }
                        } while (!uniqueUsername);

                        cout << "Enter the password: ";
                        cin >> newPassword;

                        dbManagerAdmin.insertDataAdmin(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, newUsername, newPassword);

                        dbManager.closeDB();

                        break;

                    case 2:
                        dbManager.openDB();

                        existsOrNot = dbManagerAdmin.checkIfAnyAdminExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no admin!\n";

                            adminMenu();
                        }

                        dbManagerAdmin.getAllAdminData(dbManager.DB);

                        do
                        {
                            cout << "\nEnter the id you want to update: ";
                            cin >> id;
                            cout << "\n";

                            existsOrNot = dbManagerAdmin.checkIfAnyAdminExistsBasedOnId(dbManager.DB, to_string(id));

                            if (!existsOrNot)
                            {
                                cout << "Enter exists ID!\n";
                            }
                        } while (!existsOrNot); 

                        oldUsername = dbManagerAdmin.getAdminUsernameBasedOnId(dbManager.DB, to_string(id));

                        do
                        {
                            cout << "Enter the new username: ";
                            cin >> newUsername;

                            if(newUsername!=oldUsername) 
                            {

                                uniqueUsername = dbManager.checkIfUsernameUnique(dbManager.DB, newUsername);

                                if (!uniqueUsername)
                                {
                                    cout << "\nUsername was taken!\n\n";
                                }
                            }
                            else
                            {
                                break;
                            }
                            
                        } while (!uniqueUsername);

                        
                        cout << "Enter the new password: ";
                        cin >> newPassword;

                        dbManagerAdmin.updateDataAdmin(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(id), newUsername, newPassword);

                        if (currentAdmin.id==id)
                        {
                            currentAdmin = Admin(id, newUsername, newPassword);

                            cout << "You just updated yourself!\n";

                            adminMenu();
                        }

                        dbManager.closeDB();
                        
                        break;

                    case 3:
                        dbManager.openDB();

                        existsOrNot = dbManagerAdmin.checkIfAnyAdminExists(dbManager.DB);

                        if(!existsOrNot) 
                        {
                            cout << "\nThere is no admin!\n";

                            adminMenu();
                        }

                        dbManagerAdmin.getAllAdminData(dbManager.DB);

                        do
                        {
                            cout << "\nEnter the id you want to delete: ";
                            cin >> id;
                            cout << "\n";

                            existsOrNot = dbManagerAdmin.checkIfAnyAdminExistsBasedOnId(dbManager.DB, to_string(id));

                            if (!existsOrNot)
                            {
                                cout << "Enter exists ID!\n";
                            }
                        } while (!existsOrNot); 

                        dbManagerAdmin.deleteDataAdmin(dbManager.DB, dbManager.messageError, dbManager.sql, dbManager.exit, to_string(id));

                        if (currentAdmin.id==id)
                        {
                            cout << "You just deleted yourself lol!\n";

                            defaultMenu();
                        }

                        dbManager.closeDB();
                        
                        break;

                    case 4:
                        adminMenu();
                        break;
                    
                    default:
                        cout << "Error!\n";
                        break;
                    }
                } while (true);  

                break;

            case 7:
                cout << "\n";

                dbManager.openDB();

                existsOrNot = dbManagerProduct.checkIfAnyProductExists(dbManager.DB);

                if(!existsOrNot) 
                {
                    cout << "\nThere is no product!\n";

                    adminMenu();
                }

                listOfProduct = dbManagerInventory.getListOfProduct(dbManager.DB);

                dbManager.closeDB();

                productList.addEveryProductInVector(listOfProduct);

                productList.printList();

                break;

            case 8:
                existsOrNot = dbManagerOrder.checkIfAnyOrderExists(dbManager.DB);

                if(!existsOrNot) 
                {
                    cout << "\nThere is no order!\n";

                    adminMenu();
                }

                dbManagerOrder.getMostPopularItem(dbManager.DB);
                break;

            case 9:
                currentAdmin.checkInformation(currentAdmin);
                break;

            case 10:
                defaultMenu();
                break;
                
            default:
                cout << "Error!\n";
                break;
            }
        } while (true);
    }
};

int main(int argc, char** argv) {
    Shop shop = Shop();
    DBManager dbManager;

    dbManager.openDB();
    
    dbManager.sql = "CREATE TABLE CUSTOMER("
    "CUSTOMER_ID INTEGER PRIMARY KEY NOT NULL, "
    "USERNAME TEXT NOT NULL, "
    "PASSWORD TEXT NOT NULL, "
    "CASH DECIMAL(10,2) NOT NULL );";

    dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);

    dbManager.sql = "CREATE TABLE ADMIN("
    "ADMIN_ID INTEGER PRIMARY KEY NOT NULL, "
    "USERNAME TEXT NOT NULL, "
    "PASSWORD TEXT NOT NULL );";

    dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);

    dbManager.sql = "CREATE TABLE SUPPLIER("
    "SUPPLIER_ID INTEGER PRIMARY KEY NOT NULL, "
    "NAME TEXT NOT NULL, "
    "CONTACT_INFO TEXT NOT NULL );";

    dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);

    dbManager.sql = "CREATE TABLE CATEGORY("
    "CATEGORY_ID INTEGER PRIMARY KEY NOT NULL, "
    "TYPE TEXT NOT NULL );";

    dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);

    dbManager.sql = "CREATE TABLE PRODUCT("
    "PRODUCT_ID INTEGER PRIMARY KEY NOT NULL, "
    "SUPPLIER_ID INT NOT NULL," 
    "CATEGORY_ID INT NOT NULL, "
    "NAME TEXT NOT NULL, "
    "PRICE DECIMAL(10,2) NOT NULL, "
    "FOREIGN KEY (SUPPLIER_ID) REFERENCES SUPPLIER(SUPPLIER_ID) ON DELETE CASCADE, "
    "FOREIGN KEY (CATEGORY_ID) REFERENCES CATEGORY(CATEGORY_ID) ON DELETE CASCADE);";

    dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);

    dbManager.sql = "CREATE TABLE INVENTORY("
    "INVENTORY_ID INTEGER PRIMARY KEY NOT NULL, "
    "PRODUCT_ID INT NOT NULL," 
    "QUANTITY INT NOT NULL, "
    "FOREIGN KEY (PRODUCT_ID) REFERENCES PRODUCT(PRODUCT_ID) ON DELETE CASCADE);";

    dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);

    dbManager.sql = "CREATE TRIGGER after_product_insert "
            "AFTER INSERT ON PRODUCT "
            "BEGIN "
            "INSERT INTO INVENTORY (PRODUCT_ID, QUANTITY) "
            "VALUES (NEW.PRODUCT_ID, 0); "
            "END;";

    dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);

    dbManager.sql = "CREATE TABLE \"ORDER\"("
    "ORDER_ID INTEGER PRIMARY KEY NOT NULL, "
    "CUSTOMER_ID INT NOT NULL," 
    "PRODUCT_ID INT NOT NULL, "
    "QUANTITY INT NOT NULL, "
    "TOTAL_PRICE DECIMAL(10,2) NOT NULL, "
    "FOREIGN KEY (CUSTOMER_ID) REFERENCES CUSTOMER(CUSTOMER_ID) ON DELETE CASCADE, "
    "FOREIGN KEY (PRODUCT_ID) REFERENCES PRODUCT(PRODUCT_ID) ON DELETE CASCADE);";

    dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);

    dbManager.sql = "PRAGMA foreign_keys = ON";

    dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);

    //testing

    // dbManager.sql = "INSERT INTO CUSTOMER(USERNAME, PASSWORD, CASH) VALUES('cus1', 'cus1', '0');";
    // dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);
    // dbManager.sql = "INSERT INTO CUSTOMER(USERNAME, PASSWORD, CASH) VALUES('cus2', 'cus2', '0');";
    // dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);
    // dbManager.sql = "INSERT INTO CUSTOMER(USERNAME, PASSWORD, CASH) VALUES('cus3', 'cus3', '0');";
    // dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);

    // dbManager.sql = "INSERT INTO ADMIN(USERNAME, PASSWORD) VALUES('admin1', 'admin1');";
    // dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);
    // dbManager.sql = "INSERT INTO ADMIN(USERNAME, PASSWORD) VALUES('admin2', 'admin2');";
    // dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);
    // dbManager.sql = "INSERT INTO ADMIN(USERNAME, PASSWORD) VALUES('admin3', 'admin3');";
    // dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);

    // dbManager.sql = "INSERT INTO SUPPLIER(NAME, CONTACT_INFO) VALUES('sup1', '011');";
    // dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);
    // dbManager.sql = "INSERT INTO SUPPLIER(NAME, CONTACT_INFO) VALUES('sup2', '012');";
    // dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);
    // dbManager.sql = "INSERT INTO SUPPLIER(NAME, CONTACT_INFO) VALUES('sup3', '013');";
    // dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);

    // dbManager.sql = "INSERT INTO CATEGORY(TYPE) VALUES('keyboard');";
    // dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);
    // dbManager.sql = "INSERT INTO CATEGORY(TYPE) VALUES('mouse');";
    // dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);
    // dbManager.sql = "INSERT INTO CATEGORY(TYPE) VALUES('laptop');";
    // dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);

    // dbManager.sql = "INSERT INTO PRODUCT(SUPPLIER_ID, CATEGORY_ID, NAME, PRICE) VALUES('1', '1', 'keyboard1', '60');";
    // dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);
    // dbManager.sql = "INSERT INTO PRODUCT(SUPPLIER_ID, CATEGORY_ID, NAME, PRICE) VALUES('2', '2', 'mouse1', '50');";
    // dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);
    // dbManager.sql = "INSERT INTO PRODUCT(SUPPLIER_ID, CATEGORY_ID, NAME, PRICE) VALUES('3', '3', 'laptop1', '500');";
    // dbManager.exit = sqlite3_exec(dbManager.DB, dbManager.sql.c_str(), NULL, 0, &dbManager.messageError);

    dbManager.closeDB();

    shop.defaultMenu();

    return (0);
}