#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>
#include <vector>
#include <memory>

class Database {
private:
    sqlite3* db;
    std::string dbPath;
    
    static int callback(void* data, int argc, char** argv, char** azColName);
    
public:
    Database(const std::string& dbPath = "bank_system.db");
    ~Database();
    
    bool connect();
    void disconnect();
    bool isConnected() const;
    
    // Database setup
    bool createTables();
    
    // Customer operations
    bool insertCustomer(const std::string& firstName, const std::string& middleName, 
                       const std::string& lastName, const std::string& email,
                       const std::string& phoneNumber, const std::string& address,
                       const std::string& dob, const std::string& pin);
    
    bool validateCustomerLogin(int customerId, const std::string& pin);
    int getCustomerIdByAccountNumber(const std::string& accountNumber);
    
    // Account operations
    bool createAccount(int customerId, const std::string& accountType, double initialBalance = 0.0);
    std::vector<std::string> getCustomerAccounts(int customerId);
    double getAccountBalance(const std::string& accountNumber);
    bool updateAccountBalance(const std::string& accountNumber, double newBalance);
    std::string getAccountType(const std::string& accountNumber);
    
    // Transaction operations
    bool recordTransaction(const std::string& accountNumber, const std::string& transactionType,
                          double amount, double balanceAfter, const std::string& description = "");
    std::vector<std::string> getTransactionHistory(const std::string& accountNumber, int limit = 10);
    
    // Utility functions
    std::string generateAccountNumber();
    bool accountExists(const std::string& accountNumber);
    
    // Customer info retrieval
    struct CustomerInfo {
        int customerId;
        std::string firstName;
        std::string middleName;
        std::string lastName;
        std::string email;
        std::string phoneNumber;
        std::string address;
        std::string dob;
    };
    
    CustomerInfo getCustomerInfo(int customerId);
};

#endif