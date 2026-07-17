# Zero Trust Access Control System

**A Secure Console-Based Implementation of Zero Trust Architecture in C++**

## Project Overview

This system implements the **"Never Trust, Always Verify"** principle by requiring continuous authentication and authorization. It combines Multi-Factor Authentication, Role-Based Access Control, Session Management, and Encrypted Logging. 

## Key Features

- Multi-Factor Authentication (Password + OTP)
- Brute Force Protection (3 attempt limit)
- Role-Based Access Control (RBAC) — Admin, User, Guest
- Continuous Verification
- Thread-based Session Management with Auto Logout
- Re-authentication for Sensitive Actions
- Suspicious Activity Detection
- Encrypted Audit Logging (XOR Encryption)
- Display the risk level

## Documentation

-Full Project Proposal
-Full Project Report 
-Project Presentation

## Technologies Used

- Language: C++17
- Multithreading (for session timer)
- STL**: Vector, Map
- File Handling & XOR Encryption

## OOP Concepts Applied

- Class & Object
- Encapsulation
- Abstraction
- Constructor
- Composition
- STL Containers
- Lambda Expression
- Multithreading

##  How to Run

### Prerequisites
- Code::Blocks IDE (with GCC Compiler)

### Installation & Execution Steps

1. **Clone the Repository**
   bash
   
   git clone https://github.com/AfraAnjumOdhora/Zero-Trust-Access-Control-System.git
   cd Zero-Trust-Access-Control-System

2.Open in Code::Blocks
  -Open Code::Blocks
  -Open main.cpp or the project file

3.Build and Run
  -Press F9 or go to Build → Build and Run

## Default Login Credentials

 Role     Username     Password 
 Admin     admin        123
 User      user         123
 Guest     guest        123

## Screenshots

## Team Members

Name                StudentID    Level    Term
Afra Anjum Odhora    2404025      01       02
Mst. Bithi Khatun    2404024      01       02

## Instructor: Md Masud Rana (Lecturer)

## License
This project is for academic purposes only.
