#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include <iostream>
#include <string>
#include <vector>

class BankAccount {
private:
    std::string accountNumber;
    std::string accountType;
    double balance;
    std::string status;
    int customerId;
    std::vector<std::string> localTransactionHistory; // For temporary storage
    
public:
    BankAccount();
    BankAccount(const std::string& accountNumber, int customerId, const std::string& accountType, double initialBalance = 0.0);
    ~BankAccount();
    
    // Account number management
    void setAccountNumber(const std::string& accountNum);
    std::string getAccountNumber() const;
    
    // Balance management
    void setBalance(double bal);
    double getBalance() const;
    bool hasSufficientFunds(double amount) const;
    
    // Account type management
    void setAccountType(const std::string& type);
    std::string getAccountType() const;
    
    // Customer ID management
    void setCustomerId(int id);
    int getCustomerId() const;
    
    // Account status management
    void setStatus(const std::string& accountStatus);
    std::string getStatus() const;
    bool isActive() const;
    
    // Transaction operations (for local operations before database sync)
    bool canWithdraw(double amount) const;
    bool processWithdrawal(double amount);
    bool processDeposit(double amount);
    
    // Transaction history (local cache)
    void addTransactionToHistory(const std::string& transactionRecord);
    void clearTransactionHistory();
    std::vector<std::string> getLocalTransactionHistory() const;
    bool hasTransactionHistory() const;
    
    // Account validation
    bool isValidAccount() const;
    
    // Account information display
    std::string getAccountSummary() const;
    void displayAccountDetails() const;
    
    // Utility functions
    std::string formatBalance() const;
    std::string getAccountTypeDisplay() const;
    
    // Account limits and rules
    static const double MINIMUM_BALANCE;
    static const double MAXIMUM_BALANCE;
    static bool isValidAccountType(const std::string& type);
};

#endif