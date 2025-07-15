# Inventory Management System (C++ + MySQL)
This is a simple C++ console-based inventory management system using MySQL.
 It allows admins to manage Inventory and customers to buy shop, add items to cart, and generate bills. The system updates stock in real-time and demonstrates basic database operations in C++.

## Features

#### Admin
- Add, update, and delete products in the inventory.
- Easily manage product names, prices, and stock quantities.
- View full product list.
- Maintain stock levels.

#### Customer/User
- View available products.
- Add products to cart.
- Remove items from cart.
- View cart contents.
- Generate a final bill.

## Requirements
- MySQL Server
- C++ Compiler 
- MySQL Connector/C++

## Database Schema

```sql
CREATE TABLE products (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100),
    price DOUBLE,
    quantity INT
);
