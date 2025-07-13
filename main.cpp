#include <iostream>
#include <fstream>
#include <sstream>
#include <mysqld_error.h>
#include <iomanip>   
#include <string>
#include <vector>    
#include <algorithm>
#include <climits>
#include <windows.h>  
#include <mysql.h> 

using namespace std;

// DB credentials for connection
const char* HOST = "localhost";
const char* USER = "root";
const char* PW   = "hehehe";
const char* DB   = "inventory";

// func to log txt to file
void logToFile(const string& log) {                 
    ofstream file("inventory_log.txt", ios::app); 
    if (file.is_open()) {                           
        file << log << endl;
        file.close();
    }
}

// class to represent products 
class Product {
private:
    int id;
    string name;
    double price;
    int quantity;
public: 
    Product() {     //default constructor
        id = 0;
        name = "";
        price = 0.0;
        quantity = 0;
    }
    Product(int i, string n, double p, int q) {    // Parameterised constructor
        id = i;                                
        name = n;
        price = p;
        quantity = q;
    }
    int getId() { return id; }          //getter function
    string getName() { return name; }
    double getPrice() { return price; }
    int getQuantity() { return quantity; }
};

// VENDOR MENU
void vendorMenu(MYSQL* conn) {
    bool exit = false;
    while (!exit) {
        system("cls");
        cout <<"--------------------\n";
        cout <<"--- Vendor Menu ---\n";
        cout <<"--------------------\n";
        cout << "1. Add Product\n2. Delete Product\n3. Update Product\n4. View Products\n0. Back\n\nChoice: ";
        int ch;
        cin >> ch;
        
        if (cin.fail()) {       
		cin.clear();           
	    cin.ignore(INT_MAX,'\n');   
		continue;                   
		}
        
        if (ch == 1) {          //----Inventory addition of Products-----
            int id, qty;
            double price;
            string name;
            cout << "Enter Product ID to Add (0 to Cancel): "; cin >> id;
            if(id ==0)break;
            cin.ignore();              
            cout << "Name: ";  getline(cin, name);
            do { cout << "Price (>0): "; cin >> price; } while (price <= 0);
            do { cout << "Quantity (>=0): "; cin >> qty; } while (qty < 0);

            stringstream ss;      //create a SQL INSERT query 
            ss << "INSERT INTO products (id,name,price,quantity) VALUES ("
               << id << ",'" << name << "'," << price << "," << qty << ")";
               
            if (mysql_query(conn, ss.str().c_str())) {        
                cout << "Error: " << mysql_error(conn) << endl;
            } else {
                cout << "Added Successfully!!\n";
                logToFile("Added Product: " + name);
            }
            Sleep(1000);
        }
        else if (ch == 2) {                   //---Inventory Deletion---
        // Show available products
        	system("cls");
            cout << "--- Available Products ---\n";
            cout << left << setw(5) << "ID" << setw(20) << "Name"
             << setw(10) << "Price" << setw(10) << "Qty\n"
            "----------------------------------------\n";
           
           //SELECT query is executed to fetch all products 
        if (mysql_query(conn, "SELECT * FROM products")) {   
            cout << "Error: " << mysql_error(conn) << endl;
            Sleep(1000); 
			break;
        } else {
            MYSQL_RES* res = mysql_store_result(conn);
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res))) {
                cout << left << setw(5) << row[0]
                     << setw(20) << row[1]
                     << setw(10) << row[2]
                     << setw(10) << row[3] << "\n";
            }
            mysql_free_result(res);
        }
        	
            int id;
            cout << "\nEnter ID to delete (0 to Cancel): ";
            cin >> id; 
            if(id==0) break;
            
            stringstream ss;        //create a SQL DELETE query 
            ss << "DELETE FROM products WHERE id=" << id;
            
            if (mysql_query(conn, ss.str().c_str())) {
                cout << "Error: " << mysql_error(conn) << endl;
            } else {
                cout << " Deleted (if existed)\n";
                logToFile("Deleted Product ID: " + ss.str());
            }
            Sleep(1000);
        }
        
        else if (ch == 3) {             //---Inventory Updation---
        	// Show available products
        	 system("cls");
        cout << "--- Available Products ---\n";
        cout << left << setw(5) << "ID" << setw(20) << "Name"
             << setw(10) << "Price" << setw(10) << "Qty\n"
        "----------------------------------------------\n";

          //SELECT query is executed to fetch all products 
        if (mysql_query(conn, "SELECT * FROM products")) {
            cout << "Error: " << mysql_error(conn) << endl;
            Sleep(1000); break;
        } else {
            MYSQL_RES* res = mysql_store_result(conn);
            MYSQL_ROW row;       
            while ((row = mysql_fetch_row(res))) {
                cout << left << setw(5) << row[0]
                     << setw(20) << row[1]
                     << setw(10) << row[2]
                     << setw(10) << row[3] << "\n";
            }
            mysql_free_result(res);
        }
        	
            int id, qty;
            double price;
            cout << "\nEnter ID to update (0 to Cancel): "; cin >> id;
            if(id==0)break;
            do { cout << "New Price (>0): "; cin >> price; } while (price <= 0);
            do { cout << "New Quantity (>=0): "; cin >> qty; } while (qty < 0);

            stringstream ss;       //create an SQL UPDATE query 
            ss << "UPDATE products SET price=" << price << ",quantity=" << qty
               << " WHERE id=" << id;
            if (mysql_query(conn, ss.str().c_str())) {
                cout << "Error: " << mysql_error(conn) << endl;
            } else {
                cout << "Updated !!\n";
                logToFile("Updated Product ID: " + ss.str());
            }
            Sleep(1000);
        }
        else if (ch == 4) {             //---Show Inventory--- 
           // Show available products
            system("cls");
            cout << "--- Available Products ---\n";
            cout << left << setw(5) << "ID" << setw(20) << "Name"
                 << setw(10) << "Price" << setw(10) << "Qty\n"
            "--------------------------------------------\n";
            
			//SELECT query is executed to fetch all products      
            if (mysql_query(conn, "SELECT * FROM products")) {
                cout << "Error: " << mysql_error(conn) << endl;
            } else {
                MYSQL_RES* res = mysql_store_result(conn);
                MYSQL_ROW row;
                while ((row = mysql_fetch_row(res))) {
                    cout << left << setw(5) << row[0]
                         << setw(20) << row[1]
                         << setw(10) << row[2]
                         << setw(10) << row[3] << "\n";
                }
                mysql_free_result(res);
            }
            system("pause");
        }
        else if (ch == 0) {      //---Exits vendor menu---
            return;
        }
    }
}

// CUSTOMER MENU
void customerMenu(MYSQL* conn) {
    vector<Product> cart;
    bool exit = false;
    while (!exit) {
        system("cls");
        cout <<"----------------------\n";
        cout << "--- Customer Menu ---\n";
        cout <<"----------------------\n";
        cout << "1. View Products\n2. Add to Cart\n3. Remove from Cart\n4. View Cart\n5. Generate Bill\n0. Back\n\nChoice: ";
        int ch;
        cin >> ch;
        if (cin.fail()) { 
		cin.clear(); 
		cin.ignore(INT_MAX, '\n'); 
		continue; }

        if (ch == 1) {           //---View All Products---
            system("cls");
            cout << "--- Available Products ---\n";
            cout << left << setw(5) << "ID" << setw(20) << "Name"
                 << setw(10) << "Price\n" 
            "----------------------------------------------\n";
            
            //SELECT query is executed to fetch all products 
            if (mysql_query(conn, "SELECT * FROM products")) {
                cout << "Error: " << mysql_error(conn) << endl;
            } else {
                MYSQL_RES* res = mysql_store_result(conn);
                MYSQL_ROW row;
                while ((row = mysql_fetch_row(res))) {
                    cout << left << setw(5) << row[0]
                         << setw(20) << row[1]
                         << setw(10) << row[2]
                         << "\n";
                }
                mysql_free_result(res);
            }
            system("pause");
        }

        else if (ch == 2) {      //---Add to Cart---
        	while(true){
                // Show available products
                system("cls");
                cout << "--- Available Products ---\n";
                cout << left << setw(5) << "ID" << setw(20) << "Name"
                     << setw(10) << "Price\n"
                "----------------------------------------------\n";
                
                //SELECT query is executed to fetch all products 
                if (mysql_query(conn, "SELECT * FROM products")) {
                    cout << "Error: " << mysql_error(conn) << endl;
                    Sleep(1000);
                    break;
                } else {
                    MYSQL_RES* res = mysql_store_result(conn);
                    MYSQL_ROW row;
                    while ((row = mysql_fetch_row(res))) {
                        cout << left << setw(5) << row[0]
                             << setw(20) << row[1]
                             << setw(10) << row[2]
                             << "\n";
                    }
                    mysql_free_result(res);
                }

                int id, req;
                cout << "\nEnter Product ID to add to cart (0 to stop): ";
                cin >> id;

                if (id == 0) break;

                cout << "Enter quantity: ";
                cin >> req;

                stringstream ss;     // create select Query to get product with given id to add to cart
                ss << "SELECT name,price,quantity FROM products WHERE id=" << id;
                if (mysql_query(conn, ss.str().c_str())) {
                    cout << "Error: " << mysql_error(conn) << endl;
                    Sleep(1000);
                    continue;
                }

                MYSQL_RES* res = mysql_store_result(conn);
                MYSQL_ROW row = mysql_fetch_row(res);
                if (!row) {
                    cout << "Product not found.\n";
                } else {
                    int stock = atoi(row[2]);
                    if (req > stock) {    //check stock is available
                        cout << " Only " << stock << " available.\n";
                    } else {
                        double price = atof(row[1]);
                        string name = row[0];
                        
            // Update inventory in DB (decrease stock) 
            stringstream updateStock;
            updateStock << "UPDATE products SET quantity = quantity - " << req << " WHERE id = " << id;
            
			if (mysql_query(conn, updateStock.str().c_str())) {
                cout << "Error updating stock: " << mysql_error(conn) << endl;
                mysql_free_result(res);
                continue; 
            }             
            
            // Check if already in cart
                bool found = false;
                        for (int i = 0; i < cart.size(); ++i) {
                            if (cart[i].getId() == id) {
                                int newQty = cart[i].getQuantity() + req;
                                cart[i] = Product(id, name, price, newQty);
                                found = true;
                                break;
                            }
                        }
                        if (!found) {     // Not in cart yet, push item into cart
                            Product p(id, name, price, req);
                            cart.push_back(p);
                        }
                        cout << "\nAdded to cart: " << name << " (Qty: " << req << ")\n";
                    }
                }
                mysql_free_result(res);

                cout << "\nDo you want to add another product? (y/n): ";
                char choice;
                cin >> choice;
                if (choice != 'y' && choice != 'Y') break;
            
            Sleep(1000);
        }
    }

        else if (ch == 3) {      //---Remove from Cart---
            if (cart.empty()) {
                cout << " Cart is empty. Nothing to remove.\n";
                Sleep(1000);
                continue;
            }

            // Show cart items before removal
            system("cls");
            cout << "--- Your Cart ---\n";
            cout << left << setw(5) << "ID" << setw(20) << "Name"
                 << setw(10) << "Price" << setw(5) << "Qty\n"
            "---------------------------------------------------------------\n";

            for (int i = 0; i < cart.size(); ++i) {
                int id = cart[i].getId();
                string name = cart[i].getName();
                double price = cart[i].getPrice();
                int qty = cart[i].getQuantity();
                
                cout << left << setw(5) << id
                     << setw(20) << name
                     << setw(10) << price
                     << setw(5) << qty << "\n";
            }
            cout << "--------------------------------------------------------\n";

            // Ask which product to remove
            int idToRemove;
            cout << "\nEnter Product ID to remove from cart: ";
            cin >> idToRemove;

            bool found = false;
            for (int i = 0; i < cart.size(); ++i) {
                if (cart[i].getId() == idToRemove) {
                    int qtyToRestore = cart[i].getQuantity();
 
                // Restore quantity in database
                    stringstream restoreStock;      //create SQL query to restore quantity in db
                    restoreStock << "UPDATE products SET quantity = quantity + "
                     << qtyToRestore << " WHERE id = " << idToRemove;
                    if (mysql_query(conn, restoreStock.str().c_str())) {
                        cout << "Error restoring stock: " << mysql_error(conn) << endl;
                    }
                    cart.erase(cart.begin() + i);
                    found = true;
                    cout << "\nRemoved product with ID " << idToRemove << " from cart.\n";
                    break;
                    }
               }
               if (!found) {
                cout << " Product ID not found in your cart.\n";
               }
            Sleep(1000);
        }

        else if (ch == 4) {      //-----View Cart ------
            system("cls");
            cout << left << setw(5) << "ID" << setw(20) << "Name"
                 << setw(10) << "Price" << setw(5) << "Qty\n";
            cout <<"--------------------------------------------\n";
            
            for (int i = 0; i < cart.size(); i++) {
                Product p = cart[i];
                cout << left << setw(5) << p.getId()
                     << setw(20) << p.getName()
                     << setw(10) << p.getPrice()
                     << setw(5) << p.getQuantity()<<"\n";
            }
            cout << "------------------------------------------\n";
            system("pause");
        }

        else if (ch == 5) {     //generate Bill
            system("cls");
            cout << "--- Final Bill ---\n";
            cout << left << setw(5) << "ID" << setw(20) << "Name"
                 << setw(10) << "Price" << setw(5) << "Qty"
                 << setw(10)<< "Total " << "\n";
            cout << "---------------------------------------------------------\n";

            double grand = 0.0;
            for (int i = 0; i < cart.size(); i++) {
                Product p = cart[i];
                double total = p.getPrice() * p.getQuantity();
                grand += total;
                cout << left << setw(5) << p.getId()
                     << setw(20) << p.getName()
                     << setw(10) << fixed << setprecision(2) << p.getPrice()
                     << setw(5) << p.getQuantity()
                     << setw(10)<< fixed << setprecision(2)<< total << "\n";
            }
            cout << "---------------------------------------------------------\n";
            cout <<right<<setw(40)<<"Grand Total: " << fixed << setprecision(2) << grand << "\n";

            stringstream ss;      //log bill in file
            ss << grand;
            logToFile("Generated bill: total=" + ss.str());

            //clear cart or not
            char confirm;
            cout << "\nDo you want to clear the cart after billing? (y/n): ";
            cin >> confirm;
            if (confirm == 'y' || confirm == 'Y') {
              cart.clear();
              cout << "Cart cleared.\n";
            } else {
              cout << "Shop More!!.\n";
            }
            system("pause");
        }

        else if (ch == 0) {  //---exit Customer menu---
            return;
        }
    }
}

//---- MAIN FUNCTION----                       
int main() {
    MYSQL* conn = mysql_init(NULL);     // Initialize MySQL connection object 
    
    if (!mysql_real_connect(conn, HOST, USER, PW, DB, 3306, NULL, 0)) {   
        cout << "Connection failed: " << mysql_error(conn) << endl;
        return 1;    
    }

    cout << "Connected to DB!" << endl;
    Sleep(1000);

    bool exit = false;
    while (!exit) {
        system("cls");                    //clears screen
        cout << "-----------------------------------\n";
        cout << " INVENTORY MANAGEMENT SYSTEM \n";
        cout << "-----------------------------------\n";
        cout << "--- Main Menu : ---\n1. Vendor Menu\n2. Customer Menu\n0. Exit\nChoice: ";
        int ch;
        cin >> ch;
        if (ch == 1){
		vendorMenu(conn);
        }
        else if (ch == 2){
		 customerMenu(conn);
    	} 
        else if (ch == 0) {	
		cout<<"Thankyou for Shopping !!"<<endl;
		exit=true;
	    }	
    }

    mysql_close(conn);    //close db connection
    return 0;
}
