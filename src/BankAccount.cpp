#include "BankAccount.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

const double BankAccount::MINIMUM_BALANCE = 0.0;
const double BankAccount::MAXIMUM_BALANCE = 1000000.0;

BankAccount::BankAccount() : balance(0.0), customerId(-1), status("INACTIVE") {}

BankAccount::BankAccount(const std::string& accountNumber, int customerId, 
                        const std::string& accountType, double initialBalance)
    : accountNumber(accountNumber), customerId(customerId), accountType(accountType), 
      balance(initialBalance), status("ACTIVE") {}

BankAccount::~BankAccount() {}

void BankAccount::setAccountNumber(const std::string& accountNum) {
    accountNumber = accountNum;
}

std::string BankAccount::getAccountNumber() const {
    return accountNumber;
}

void BankAccount::setBalance(double bal) {
    if (bal >= MINIMUM_BALANCE && bal <= MAXIMUM_BALANCE) {
        balance = bal;
    }
}

double BankAccount::getBalance() const {
    return balance;
}

bool BankAccount::hasSufficientFunds(double amount) const {
    return balance >= amount && amount > 0;
}

void BankAccount::setAccountType(const std::string& type) {
    if (isValidAccountType(type)) {
        accountType = type;
    }
}

std::string BankAccount::getAccountType() const {
    return accountType;
}

void BankAccount::setCustomerId(int id) {
    customerId = id;
}

int BankAccount::getCustomerId() const {
    return customerId;
}

void BankAccount::setStatus(const std::string& accountStatus) {
    if (accountStatus == "ACTIVE" || accountStatus == "INACTIVE" || 
        accountStatus == "FROZEN" || accountStatus == "CLOSED") {
        status = accountStatus;
    }
}

std::string BankAccount::getStatus() const {
    return status;
}

bool BankAccount::isActive() const {
    return status == "ACTIVE";
}

bool BankAccount::canWithdraw(double amount) const {
    return isActive() && hasSufficientFunds(amount) && amount > 0;
}

bool BankAccount::processWithdrawal(double amount) {
    if (!canWithdraw(amount)) {
        return false;
    }
    
    balance -= amount;
    
    // Add to local transaction history
    std::stringstream ss;
    ss << "WITHDRAWAL|" << std::fixed << std::setprecision(2) << amount 
       << "|" << balance << "|Cash withdrawal|" << "Local Transaction";
    addTransactionToHistory(ss.str());
    
    return true;
}

bool BankAccount::processDeposit(double amount) {
    if (!isActive() || amount <= 0 || (balance + amount) > MAXIMUM_BALANCE) {
        return false;
    }
    
    balance += amount;
    
    // Add to local transaction history
    std::stringstream ss;
    ss << "DEPOSIT|" << std::fixed << std::setprecision(2) << amount 
       << "|" << balance << "|Cash deposit|" << "Local Transaction";
    addTransactionToHistory(ss.str());
    
    return true;
}

void BankAccount::addTransactionToHistory(const std::string& transactionRecord) {
    localTransactionHistory.push_back(transactionRecord);
    
    // Keep only last 20 transactions in local cache
    if (localTransactionHistory.size() > 20) {
        localTransactionHistory.erase(localTransactionHistory.begin());
    }
}

void BankAccount::clearTransactionHistory() {
    localTransactionHistory.clear();
}

std::vector<std::string> BankAccount::getLocalTransactionHistory() const {
    return localTransactionHistory;
}

bool BankAccount::hasTransactionHistory() const {
    return !localTransactionHistory.empty();
}

bool BankAccount::isValidAccount() const {
    return !accountNumber.empty() && customerId > 0 && isValidAccountType(accountType);
}

std::string BankAccount::getAccountSummary() const {
    std::stringstream ss;
    ss << accountNumber << " (" << getAccountTypeDisplay() << ") - $" 
       << std::fixed << std::setprecision(2) << balance;
    return ss.str();
}

void BankAccount::displayAccountDetails() const {
    std::cout << "\n Account Details:" << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    std::cout << "Account Number: " << accountNumber << std::endl;
    std::cout << "Account Type: " << getAccountTypeDisplay() << std::endl;
    std::cout << "Current Balance: " << formatBalance() << std::endl;
    std::cout << "Status: " << status << std::endl;
    std::cout << "Customer ID: " << customerId << std::endl;
}

std::string BankAccount::formatBalance() const {
    std::stringstream ss;
    ss << "$" << std::fixed << std::setprecision(2) << balance;
    return ss.str();
}

std::string BankAccount::getAccountTypeDisplay() const {
    if (accountType == "Savings") {
        return " Savings Account";
    } else if (accountType == "Checkings") {
        return "💳 Checkings Account";
    }
    return accountType;
}

bool BankAccount::isValidAccountType(const std::string& type) {
    return type == "Savings" || type == "Checkings" || type == "Current" || type == "Business";
}