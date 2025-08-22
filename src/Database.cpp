#include "Database.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <random>
#include <chrono>

Database::Database(const std::string& dbPath) : db(nullptr), dbPath(dbPath) {}

Database::~Database() {
    disconnect();
}

bool Database::connect() {
    int result = sqlite3_open(dbPath.c_str(), &db);
    if (result != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    return createTables();
}

void Database::disconnect() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool Database::isConnected() const {
    return db != nullptr;
}

int Database::callback(void* data, int argc, char** argv, char** azColName) {
    std::vector<std::string>* result = static_cast<std::vector<std::string>*>(data);
    std::string row = "";
    for (int i = 0; i < argc; i++) {
        if (i > 0) row += "|";
        row += (argv[i] ? argv[i] : "NULL");
    }
    result->push_back(row);
    return 0;
}

bool Database::createTables() {
    const char* createCustomersTable = R"(
        CREATE TABLE IF NOT EXISTS customers (
            customer_id INTEGER PRIMARY KEY AUTOINCREMENT,
            first_name TEXT NOT NULL,
            middle_name TEXT,
            last_name TEXT NOT NULL,
            email TEXT UNIQUE NOT NULL,
            phone_number TEXT NOT NULL,
            address TEXT NOT NULL,
            date_of_birth TEXT NOT NULL,
            pin TEXT NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        );
    )";

    const char* createAccountsTable = R"(
        CREATE TABLE IF NOT EXISTS accounts (
            account_number TEXT PRIMARY KEY,
            customer_id INTEGER NOT NULL,
            account_type TEXT NOT NULL,
            balance REAL NOT NULL DEFAULT 0.0,
            status TEXT DEFAULT 'ACTIVE',
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (customer_id) REFERENCES customers (customer_id)
        );
    )";

    const char* createTransactionsTable = R"(
        CREATE TABLE IF NOT EXISTS transactions (
            transaction_id INTEGER PRIMARY KEY AUTOINCREMENT,
            account_number TEXT NOT NULL,
            transaction_type TEXT NOT NULL,
            amount REAL NOT NULL,
            balance_after REAL NOT NULL,
            description TEXT,
            transaction_date DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY (account_number) REFERENCES accounts (account_number)
        );
    )";

    char* errMsg = 0;
    
    if (sqlite3_exec(db, createCustomersTable, 0, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error creating customers table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    if (sqlite3_exec(db, createAccountsTable, 0, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error creating accounts table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    if (sqlite3_exec(db, createTransactionsTable, 0, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error creating transactions table: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    return true;
}

bool Database::insertCustomer(const std::string& firstName, const std::string& middleName,
                             const std::string& lastName, const std::string& email,
                             const std::string& phoneNumber, const std::string& address,
                             const std::string& dob, const std::string& pin) {
    const char* sql = R"(
        INSERT INTO customers (first_name, middle_name, last_name, email, phone_number, address, date_of_birth, pin)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?);
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, firstName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, middleName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, lastName.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, phoneNumber.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, address.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, dob.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, pin.c_str(), -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return result == SQLITE_DONE;
}

bool Database::validateCustomerLogin(int customerId, const std::string& pin) {
    const char* sql = "SELECT pin FROM customers WHERE customer_id = ?";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, customerId);
    
    bool valid = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* storedPin = (const char*)sqlite3_column_text(stmt, 0);
        if (storedPin && pin == storedPin) {
            valid = true;
        }
    }

    sqlite3_finalize(stmt);
    return valid;
}

int Database::getCustomerIdByAccountNumber(const std::string& accountNumber) {
    const char* sql = "SELECT customer_id FROM accounts WHERE account_number = ?";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1;
    }

    sqlite3_bind_text(stmt, 1, accountNumber.c_str(), -1, SQLITE_STATIC);
    
    int customerId = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        customerId = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return customerId;
}

std::string Database::generateAccountNumber() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100000000, 999999999);
    
    std::string accountNumber;
    do {
        accountNumber = std::to_string(dis(gen));
    } while (accountExists(accountNumber));
    
    return accountNumber;
}

bool Database::accountExists(const std::string& accountNumber) {
    const char* sql = "SELECT account_number FROM accounts WHERE account_number = ?";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, accountNumber.c_str(), -1, SQLITE_STATIC);
    
    bool exists = (sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    
    return exists;
}

bool Database::createAccount(int customerId, const std::string& accountType, double initialBalance) {
    std::string accountNumber = generateAccountNumber();
    
    const char* sql = R"(
        INSERT INTO accounts (account_number, customer_id, account_type, balance)
        VALUES (?, ?, ?, ?);
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, accountNumber.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, customerId);
    sqlite3_bind_text(stmt, 3, accountType.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, initialBalance);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result == SQLITE_DONE && initialBalance > 0) {
        recordTransaction(accountNumber, "DEPOSIT", initialBalance, initialBalance, "Initial deposit");
    }

    return result == SQLITE_DONE;
}

std::vector<std::string> Database::getCustomerAccounts(int customerId) {
    std::vector<std::string> accounts;
    
    const char* sql = "SELECT account_number, account_type, balance FROM accounts WHERE customer_id = ? AND status = 'ACTIVE'";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return accounts;
    }

    sqlite3_bind_int(stmt, 1, customerId);
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string accountInfo = std::string((const char*)sqlite3_column_text(stmt, 0)) + "|" +
                                 std::string((const char*)sqlite3_column_text(stmt, 1)) + "|" +
                                 std::to_string(sqlite3_column_double(stmt, 2));
        accounts.push_back(accountInfo);
    }

    sqlite3_finalize(stmt);
    return accounts;
}

double Database::getAccountBalance(const std::string& accountNumber) {
    const char* sql = "SELECT balance FROM accounts WHERE account_number = ?";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return -1.0;
    }

    sqlite3_bind_text(stmt, 1, accountNumber.c_str(), -1, SQLITE_STATIC);
    
    double balance = -1.0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        balance = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return balance;
}

bool Database::updateAccountBalance(const std::string& accountNumber, double newBalance) {
    const char* sql = "UPDATE accounts SET balance = ? WHERE account_number = ?";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_double(stmt, 1, newBalance);
    sqlite3_bind_text(stmt, 2, accountNumber.c_str(), -1, SQLITE_STATIC);
    
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return result == SQLITE_DONE;
}

std::string Database::getAccountType(const std::string& accountNumber) {
    const char* sql = "SELECT account_type FROM accounts WHERE account_number = ?";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return "";
    }

    sqlite3_bind_text(stmt, 1, accountNumber.c_str(), -1, SQLITE_STATIC);
    
    std::string accountType = "";
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        accountType = std::string((const char*)sqlite3_column_text(stmt, 0));
    }

    sqlite3_finalize(stmt);
    return accountType;
}

bool Database::recordTransaction(const std::string& accountNumber, const std::string& transactionType,
                                double amount, double balanceAfter, const std::string& description) {
    const char* sql = R"(
        INSERT INTO transactions (account_number, transaction_type, amount, balance_after, description)
        VALUES (?, ?, ?, ?, ?);
    )";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, accountNumber.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, transactionType.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, amount);
    sqlite3_bind_double(stmt, 4, balanceAfter);
    sqlite3_bind_text(stmt, 5, description.c_str(), -1, SQLITE_STATIC);

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return result == SQLITE_DONE;
}

std::vector<std::string> Database::getTransactionHistory(const std::string& accountNumber, int limit) {
    std::vector<std::string> transactions;
    
    const char* sql = R"(
        SELECT transaction_type, amount, balance_after, description, transaction_date 
        FROM transactions 
        WHERE account_number = ? 
        ORDER BY transaction_date DESC 
        LIMIT ?
    )";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return transactions;
    }

    sqlite3_bind_text(stmt, 1, accountNumber.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, limit);
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string transaction = 
            std::string((const char*)sqlite3_column_text(stmt, 0)) + "|" +
            std::to_string(sqlite3_column_double(stmt, 1)) + "|" +
            std::to_string(sqlite3_column_double(stmt, 2)) + "|" +
            std::string((const char*)sqlite3_column_text(stmt, 3)) + "|" +
            std::string((const char*)sqlite3_column_text(stmt, 4));
        transactions.push_back(transaction);
    }

    sqlite3_finalize(stmt);
    return transactions;
}

Database::CustomerInfo Database::getCustomerInfo(int customerId) {
    CustomerInfo info;
    info.customerId = -1;
    
    const char* sql = "SELECT customer_id, first_name, middle_name, last_name, email, phone_number, address, date_of_birth FROM customers WHERE customer_id = ?";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return info;
    }

    sqlite3_bind_int(stmt, 1, customerId);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        info.customerId = sqlite3_column_int(stmt, 0);
        info.firstName = std::string((const char*)sqlite3_column_text(stmt, 1));
        info.middleName = std::string((const char*)sqlite3_column_text(stmt, 2));
        info.lastName = std::string((const char*)sqlite3_column_text(stmt, 3));
        info.email = std::string((const char*)sqlite3_column_text(stmt, 4));
        info.phoneNumber = std::string((const char*)sqlite3_column_text(stmt, 5));
        info.address = std::string((const char*)sqlite3_column_text(stmt, 6));
        info.dob = std::string((const char*)sqlite3_column_text(stmt, 7));
    }

    sqlite3_finalize(stmt);
    return info;
}