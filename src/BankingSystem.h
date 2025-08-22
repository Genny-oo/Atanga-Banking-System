#ifndef BANKING_SYSTEM_H
#define BANKING_SYSTEM_H

#include "Database.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>

class BankingSystem {
private:
    std::unique_ptr<Database> database;
    int currentCustomerId;
    std::string currentAccountNumber;
    bool isLoggedIn;
    
    // Input validation helpers
    bool isValidEmail(const std::string& email) const;
    bool isValidPhoneNumber(const std::string& phone) const;
    bool isValidPin(const std::string& pin) const;
    bool isValidAmount(double amount) const;
    
    // UI helpers
    void clearScreen() const;
    void displayHeader(const std::string& title) const;
    void displayMainMenu() ;
    void displayAccountSelectionMenu(const std::vector<std::string>& accounts) const;
    
    // Transaction limits and validation
    static const double MAX_WITHDRAWAL_AMOUNT;
    static const double MIN_TRANSACTION_AMOUNT;
    static const double MAX_DAILY_WITHDRAWAL;
    
public:
    BankingSystem();
    ~BankingSystem();
    
    // System initialization
    bool initialize();
    
    // Main system loop
    void run();
    void displayWelcomeMenu();
    
    // Authentication
    bool createCustomerAccount();
    bool loginCustomer();
    void logout();
    
    // Account management
    bool createBankAccount(const std::string& accountType);
    void selectAccount();
    void displayAccountInfo() const;
    
    // Banking operations
    void deposit();
    void withdraw();
    void checkBalance() const;
    void viewTransactionHistory() const;
    void transferFunds();
    
    // Utility functions
    std::string getFullName(const Database::CustomerInfo& customer) const;
    void displayTransactionReceipt(const std::string& transactionType, double amount, double newBalance) const;
    
    // Security features
    bool verifyPin() const;
    void lockAccount();
    
    // Getters
    bool getIsLoggedIn() const { return isLoggedIn; }
    int getCurrentCustomerId() const { return currentCustomerId; }
    std::string getCurrentAccountNumber() const { return currentAccountNumber; }
};

#endif