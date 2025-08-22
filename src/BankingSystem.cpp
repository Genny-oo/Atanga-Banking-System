#include "BankingSystem.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <regex>
#include <limits>
#include <thread>
#include <chrono>

const double BankingSystem::MAX_WITHDRAWAL_AMOUNT = 10000.0;
const double BankingSystem::MIN_TRANSACTION_AMOUNT = 1.0;
const double BankingSystem::MAX_DAILY_WITHDRAWAL = 50000.0;

BankingSystem::BankingSystem() : currentCustomerId(-1), isLoggedIn(false) {
    database = std::make_unique<Database>();
}

BankingSystem::~BankingSystem() = default;

bool BankingSystem::initialize() {
    std::cout << "Initializing KNUST Banking System..." << std::endl;
    if (!database->connect()) {
        std::cerr << "Failed to connect to database!" << std::endl;
        return false;
    }
    std::cout << "Banking System initialized successfully!" << std::endl;
    return true;
}

void BankingSystem::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void BankingSystem::displayHeader(const std::string& title) const {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << std::setw(30 + title.length()/2) << title << std::endl;
    std::cout << std::string(60, '=') << std::endl;
}

void BankingSystem::run() {
    if (!initialize()) {
        return;
    }
    
    while (true) {
        if (!isLoggedIn) {
            displayWelcomeMenu();
        } else {
            displayMainMenu();
        }
    }
}

void BankingSystem::displayWelcomeMenu() {
    clearScreen();
    displayHeader("KNUST BANKING SYSTEM");
    
    std::cout << "\n Welcome to KNUST Bank" << std::endl;
    std::cout << "Your Trusted Financial Partner" << std::endl;
    std::cout << "\n1.  Create New Customer Account" << std::endl;
    std::cout << "2.  Login to Existing Account" << std::endl;
    std::cout << "3.  Exit System" << std::endl;
    std::cout << "\nPlease select an option (1-3): ";
    
    int choice;
    std::cin >> choice;
    std::cin.ignore();
    
    switch (choice) {
        case 1:
            createCustomerAccount();
            break;
        case 2:
            loginCustomer();
            break;
        case 3:
            std::cout << "\nThank you for choosing KNUST Bank!" << std::endl;
            std::cout << "Have a great day! " << std::endl;
            exit(0);
            break;
        default:
            std::cout << " Invalid option. Please try again." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            break;
    }
}

void BankingSystem::displayMainMenu() {
    clearScreen();
    displayHeader("ATANGA BANK - ONLINE BANKING");
    
    Database::CustomerInfo customer = database->getCustomerInfo(currentCustomerId);
    std::cout << "\n Welcome back, " << getFullName(customer) << "!" << std::endl;
    
    if (!currentAccountNumber.empty()) {
        std::cout << " Active Account: " << currentAccountNumber 
                  << " (" << database->getAccountType(currentAccountNumber) << ")" << std::endl;
        std::cout << " Current Balance: $" << std::fixed << std::setprecision(2) 
                  << database->getAccountBalance(currentAccountNumber) << std::endl;
    }
    
    std::cout << "\n Banking Services:" << std::endl;
    std::cout << "1.  Switch Account" << std::endl;
    std::cout << "2.  Check Balance" << std::endl;
    std::cout << "3.  Deposit Money" << std::endl;
    std::cout << "4.  Withdraw Money" << std::endl;
    std::cout << "5.  Transfer Funds" << std::endl;
    std::cout << "6.  Transaction History" << std::endl;
    std::cout << "7.  Account Information" << std::endl;
    std::cout << "8.  Open New Account" << std::endl;
    std::cout << "9.  Logout" << std::endl;
    std::cout << "\nSelect a service (1-9): ";
    
    int choice;
    std::cin >> choice;
    std::cin.ignore();
    
    switch (choice) {
        case 1:
            selectAccount();
            break;
        case 2:
            checkBalance();
            break;
        case 3:
            deposit();
            break;
        case 4:
            withdraw();
            break;
        case 5:
            transferFunds();
            break;
        case 6:
            viewTransactionHistory();
            break;
        case 7:
            displayAccountInfo();
            break;
        case 8:
            {
                std::string accountType;
                std::cout << "Enter account type (Savings/Checkings): ";
                std::getline(std::cin, accountType);
                createBankAccount(accountType);
            }
            break;
        case 9:
            logout();
            break;
        default:
            std::cout << " Invalid option. Please try again." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            break;
    }
}

bool BankingSystem::isValidEmail(const std::string& email) const {
    const std::regex emailPattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return std::regex_match(email, emailPattern);
}

bool BankingSystem::isValidPhoneNumber(const std::string& phone) const {
    return phone.length() >= 10 && phone.length() <= 15 && 
           std::all_of(phone.begin(), phone.end(), ::isdigit);
}

bool BankingSystem::isValidPin(const std::string& pin) const {
    return pin.length() == 4 && std::all_of(pin.begin(), pin.end(), ::isdigit);
}

bool BankingSystem::isValidAmount(double amount) const {
    return amount >= MIN_TRANSACTION_AMOUNT && amount <= MAX_WITHDRAWAL_AMOUNT;
}

bool BankingSystem::createCustomerAccount() {
    clearScreen();
    displayHeader("NEW CUSTOMER REGISTRATION");
    
    std::string firstName, middleName, lastName, email, phoneNumber, address, dob, pin, confirmPin;
    
    std::cout << "\n Please provide your details:" << std::endl;
    std::cout << "\nFirst Name: ";
    std::getline(std::cin, firstName);
    
    std::cout << "Middle Name (optional): ";
    std::getline(std::cin, middleName);
    
    std::cout << "Last Name: ";
    std::getline(std::cin, lastName);
    
    // Email validation
    do {
        std::cout << "Email Address: ";
        std::getline(std::cin, email);
        if (!isValidEmail(email)) {
            std::cout << " Invalid email format. Please try again." << std::endl;
        }
    } while (!isValidEmail(email));
    
    // Phone number validation
    do {
        std::cout << "Phone Number (10-15 digits): ";
        std::getline(std::cin, phoneNumber);
        if (!isValidPhoneNumber(phoneNumber)) {
            std::cout << " Invalid phone number. Please enter 10-15 digits only." << std::endl;
        }
    } while (!isValidPhoneNumber(phoneNumber));
    
    std::cout << "Complete Address: ";
    std::getline(std::cin, address);
    
    std::cout << "Date of Birth (DD/MM/YYYY): ";
    std::getline(std::cin, dob);
    
    // PIN validation
    do {
        std::cout << "Create a 4-digit Security PIN: ";
        std::getline(std::cin, pin);
        if (!isValidPin(pin)) {
            std::cout << " PIN must be exactly 4 digits. Please try again." << std::endl;
            continue;
        }
        
        std::cout << "Confirm your PIN: ";
        std::getline(std::cin, confirmPin);
        if (pin != confirmPin) {
            std::cout << " PINs do not match. Please try again." << std::endl;
        }
    } while (!isValidPin(pin) || pin != confirmPin);
    
    if (database->insertCustomer(firstName, middleName, lastName, email, phoneNumber, address, dob, pin)) {
        std::cout << "\n Customer account created successfully!" << std::endl;
        std::cout << " Welcome to KNUST Bank family!" << std::endl;
        
        std::cout << "\nWould you like to open a bank account now? (y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == 'y' || choice == 'Y') {
            std::string accountType;
            do {
                std::cout << "\nChoose account type:" << std::endl;
                std::cout << "1. Savings Account" << std::endl;
                std::cout << "2. Checkings Account" << std::endl;
                std::cout << "Enter choice (1-2): ";
                int accountChoice;
                std::cin >> accountChoice;
                std::cin.ignore();
                
                if (accountChoice == 1) {
                    accountType = "Savings";
                    break;
                } else if (accountChoice == 2) {
                    accountType = "Checkings";
                    break;
                } else {
                    std::cout << " Invalid choice. Please try again." << std::endl;
                }
            } while (true);
            
            // Get customer ID by email (since we just created the customer)
            // In a real system, you'd get this from the insert operation
            std::cout << "\nOpening your " << accountType << " account..." << std::endl;
        }
    } else {
        std::cout << " Failed to create customer account. Please try again." << std::endl;
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
    return true;
}

bool BankingSystem::loginCustomer() {
    clearScreen();
    displayHeader("CUSTOMER LOGIN");
    
    std::string accountNumber, pin;
    
    std::cout << "\n Please enter your credentials:" << std::endl;
    std::cout << "\nAccount Number: ";
    std::getline(std::cin, accountNumber);
    
    std::cout << "PIN: ";
    std::getline(std::cin, pin);
    
    int customerId = database->getCustomerIdByAccountNumber(accountNumber);
    if (customerId != -1 && database->validateCustomerLogin(customerId, pin)) {
        currentCustomerId = customerId;
        currentAccountNumber = accountNumber;
        isLoggedIn = true;
        
        std::cout << "\n Login successful!" << std::endl;
        std::cout << " Welcome to KNUST Bank Online Banking!" << std::endl;
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return true;
    } else {
        std::cout << "\n Invalid account number or PIN." << std::endl;
        std::cout << "Please check your credentials and try again." << std::endl;
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
        return false;
    }
}

void BankingSystem::logout() {
    currentCustomerId = -1;
    currentAccountNumber = "";
    isLoggedIn = false;
    
    clearScreen();
    displayHeader("LOGOUT SUCCESSFUL");
    std::cout << "\n You have been logged out successfully." << std::endl;
    std::cout << " Thank you for banking with KNUST Bank!" << std::endl;
    std::cout << "Have a great day! " << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

void BankingSystem::selectAccount() {
    clearScreen();
    displayHeader("SELECT ACCOUNT");
    
    std::vector<std::string> accounts = database->getCustomerAccounts(currentCustomerId);
    
    if (accounts.empty()) {
        std::cout << "\n No accounts found." << std::endl;
        std::cout << "Please open a bank account first." << std::endl;
        std::cout << "\nPress Enter to continue...";
        std::cin.get();
        return;
    }
    
    displayAccountSelectionMenu(accounts);
    
    std::cout << "\nSelect account (1-" << accounts.size() << "): ";
    int choice;
    std::cin >> choice;
    std::cin.ignore();
    
    if (choice >= 1 && choice <= accounts.size()) {
        std::string selectedAccount = accounts[choice - 1];
        size_t pos = selectedAccount.find('|');
        currentAccountNumber = selectedAccount.substr(0, pos);
        
        std::cout << "\n Account selected: " << currentAccountNumber << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.get();
    } else {
        std::cout << " Invalid selection. Please try again." << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.get();
    }
}

void BankingSystem::displayAccountSelectionMenu(const std::vector<std::string>& accounts) const {
    std::cout << "\n Your Bank Accounts:" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    for (size_t i = 0; i < accounts.size(); ++i) {
        std::string account = accounts[i];
        size_t pos1 = account.find('|');
        size_t pos2 = account.find('|', pos1 + 1);
        
        std::string accountNumber = account.substr(0, pos1);
        std::string accountType = account.substr(pos1 + 1, pos2 - pos1 - 1);
        std::string balance = account.substr(pos2 + 1);
        
        std::cout << (i + 1) << ".  " << accountNumber << " (" << accountType << ")"
                  << "\n    Balance: $" << std::fixed << std::setprecision(2) << std::stod(balance) << std::endl;
        std::cout << std::string(40, '-') << std::endl;
    }
}

bool BankingSystem::createBankAccount(const std::string& accountType) {
    clearScreen();
    displayHeader("OPEN NEW BANK ACCOUNT");
    
    if (accountType != "Savings" && accountType != "Checkings") {
        std::cout << " Invalid account type. Please choose Savings or Checkings." << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return false;
    }
    
    std::cout << "\n Opening " << accountType << " Account..." << std::endl;
    
    double initialDeposit = 0.0;
    std::cout << "\nMinimum opening deposit: $" << MIN_TRANSACTION_AMOUNT << std::endl;
    std::cout << "Enter initial deposit amount: $";
    std::cin >> initialDeposit;
    std::cin.ignore();
    
    if (initialDeposit < MIN_TRANSACTION_AMOUNT) {
        std::cout << " Initial deposit must be at least $" << MIN_TRANSACTION_AMOUNT << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return false;
    }
    
    if (database->createAccount(currentCustomerId, accountType, initialDeposit)) {
        std::cout << "\n " << accountType << " account opened successfully!" << std::endl;
        std::cout << " Initial deposit: $" << std::fixed << std::setprecision(2) << initialDeposit << std::endl;
        
        // Auto-select the new account if it's the first one
        std::vector<std::string> accounts = database->getCustomerAccounts(currentCustomerId);
        if (accounts.size() == 1) {
            size_t pos = accounts[0].find('|');
            currentAccountNumber = accounts[0].substr(0, pos);
            std::cout << " Account automatically selected: " << currentAccountNumber << std::endl;
        }
        
        std::cout << "\n Welcome to KNUST Bank family!" << std::endl;
    } else {
        std::cout << " Failed to create account. Please try again." << std::endl;
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
    return true;
}

void BankingSystem::deposit() {
    if (currentAccountNumber.empty()) {
        std::cout << " Please select an account first." << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return;
    }
    
    clearScreen();
    displayHeader("DEPOSIT MONEY");
    
    double currentBalance = database->getAccountBalance(currentAccountNumber);
    std::cout << "\n Account: " << currentAccountNumber << std::endl;
    std::cout << " Current Balance: $" << std::fixed << std::setprecision(2) << currentBalance << std::endl;
    
    double amount;
    std::cout << "\n Enter deposit amount: $";
    std::cin >> amount;
    std::cin.ignore();
    
    if (amount < MIN_TRANSACTION_AMOUNT) {
        std::cout << " Minimum deposit amount is $" << MIN_TRANSACTION_AMOUNT << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return;
    }
    
    double newBalance = currentBalance + amount;
    
    if (database->updateAccountBalance(currentAccountNumber, newBalance) &&
        database->recordTransaction(currentAccountNumber, "DEPOSIT", amount, newBalance, "Cash deposit")) {
        
        displayTransactionReceipt("DEPOSIT", amount, newBalance);
        std::cout << "\n Deposit successful!" << std::endl;
        std::cout << " Funds have been added to your account." << std::endl;
    } else {
        std::cout << " Deposit failed. Please try again." << std::endl;
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void BankingSystem::withdraw() {
    if (currentAccountNumber.empty()) {
        std::cout << " Please select an account first." << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return;
    }
    
    clearScreen();
    displayHeader("WITHDRAW MONEY");
    
    double currentBalance = database->getAccountBalance(currentAccountNumber);
    std::cout << "\n Account: " << currentAccountNumber << std::endl;
    std::cout << " Available Balance: $" << std::fixed << std::setprecision(2) << currentBalance << std::endl;
    std::cout << " Daily withdrawal limit: $" << MAX_WITHDRAWAL_AMOUNT << std::endl;
    
    double amount;
    std::cout << "\n Enter withdrawal amount: $";
    std::cin >> amount;
    std::cin.ignore();
    
    if (amount < MIN_TRANSACTION_AMOUNT) {
        std::cout << " Minimum withdrawal amount is $" << MIN_TRANSACTION_AMOUNT << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return;
    }
    
    if (amount > MAX_WITHDRAWAL_AMOUNT) {
        std::cout << " Maximum withdrawal amount is $" << MAX_WITHDRAWAL_AMOUNT << std::endl;
        std::cout << "Please contact the bank for larger withdrawals." << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return;
    }
    
    if (amount > currentBalance) {
        std::cout << " Insufficient funds!" << std::endl;
        std::cout << " Your available balance is: $" << std::fixed << std::setprecision(2) << currentBalance << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return;
    }
    
    double newBalance = currentBalance - amount;
    
    if (database->updateAccountBalance(currentAccountNumber, newBalance) &&
        database->recordTransaction(currentAccountNumber, "WITHDRAWAL", amount, newBalance, "Cash withdrawal")) {
        
        displayTransactionReceipt("WITHDRAWAL", amount, newBalance);
        std::cout << "\n Withdrawal successful!" << std::endl;
        std::cout << " Please collect your cash from the dispenser." << std::endl;
    } else {
        std::cout << " Withdrawal failed. Please try again." << std::endl;
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void BankingSystem::checkBalance() const {
    if (currentAccountNumber.empty()) {
        std::cout << " Please select an account first." << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return;
    }
    
    clearScreen();
    displayHeader("ACCOUNT BALANCE");
    
    double balance = database->getAccountBalance(currentAccountNumber);
    std::string accountType = database->getAccountType(currentAccountNumber);
    
    std::cout << "\n Account Number: " << currentAccountNumber << std::endl;
    std::cout << " Account Type: " << accountType << std::endl;
    std::cout << " Current Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
    
    if (balance < 100) {
        std::cout << "\n  Low balance alert! Consider making a deposit." << std::endl;
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void BankingSystem::viewTransactionHistory() const {
    if (currentAccountNumber.empty()) {
        std::cout << " Please select an account first." << std::endl;
        std::cout << "Press Enter to continue...";
        std::cin.get();
        return;
    }
    
    clearScreen();
    displayHeader("TRANSACTION HISTORY");
    
    std::cout << "\n Account: " << currentAccountNumber << std::endl;
    std::cout << " Last 10 Transactions" << std::endl;
    std::cout << std::string(90, '-') << std::endl;
    
    std::vector<std::string> transactions = database->getTransactionHistory(currentAccountNumber, 10);
    
    if (transactions.empty()) {
        std::cout << " No transaction history available." << std::endl;
    } else {
        std::cout << std::left << std::setw(12) << "Type" 
                  << std::setw(12) << "Amount" 
                  << std::setw(15) << "Balance After"
                  << std::setw(25) << "Description"
                  << "Date/Time" << std::endl;
        std::cout << std::string(90, '-') << std::endl;
        
        for (const auto& transaction : transactions) {
            size_t pos1 = transaction.find('|');
            size_t pos2 = transaction.find('|', pos1 + 1);
            size_t pos3 = transaction.find('|', pos2 + 1);
            size_t pos4 = transaction.find('|', pos3 + 1);
            
            std::string type = transaction.substr(0, pos1);
            std::string amount = transaction.substr(pos1 + 1, pos2 - pos1 - 1);
            std::string balanceAfter = transaction.substr(pos2 + 1, pos3 - pos2 - 1);
            std::string description = transaction.substr(pos3 + 1, pos4 - pos3 - 1);
            std::string date = transaction.substr(pos4 + 1);
            
            std::string emoji = (type == "DEPOSIT") ? "📥" : "📤";
            
            std::cout << std::left << emoji << std::setw(10) << type 
                      << "$" << std::setw(11) << std::fixed << std::setprecision(2) << std::stod(amount)
                      << "$" << std::setw(14) << std::fixed << std::setprecision(2) << std::stod(balanceAfter)
                      << std::setw(25) << description
                      << date << std::endl;
        }
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void BankingSystem::transferFunds() {
    clearScreen();
    displayHeader("FUND TRANSFER");
    
    std::cout << "\n Fund Transfer Service" << std::endl;
    std::cout << "This feature will be available in the next update." << std::endl;
    std::cout << "Coming soon: Transfer between your accounts and to other customers!" << std::endl;
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

void BankingSystem::displayAccountInfo() const {
    if (currentCustomerId == -1) {
        std::cout << " Please login first." << std::endl;
        return;
    }
    
    clearScreen();
    displayHeader("ACCOUNT INFORMATION");
    
    Database::CustomerInfo customer = database->getCustomerInfo(currentCustomerId);
    std::vector<std::string> accounts = database->getCustomerAccounts(currentCustomerId);
    
    std::cout << "\n Customer Information:" << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    std::cout << "Name: " << getFullName(customer) << std::endl;
    std::cout << "Email: " << customer.email << std::endl;
    std::cout << "Phone: " << customer.phoneNumber << std::endl;
    std::cout << "Address: " << customer.address << std::endl;
    std::cout << "Date of Birth: " << customer.dob << std::endl;
    std::cout << "Customer ID: " << customer.customerId << std::endl;
    
    std::cout << "\n Bank Accounts:" << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    
    if (accounts.empty()) {
        std::cout << " No bank accounts found." << std::endl;
    } else {
        double totalBalance = 0.0;
        for (const auto& account : accounts) {
            size_t pos1 = account.find('|');
            size_t pos2 = account.find('|', pos1 + 1);
            
            std::string accountNumber = account.substr(0, pos1);
            std::string accountType = account.substr(pos1 + 1, pos2 - pos1 - 1);
            double balance = std::stod(account.substr(pos2 + 1));
            totalBalance += balance;
            
            std::string emoji = (accountType == "Savings") ? "🏛️" : "💳";
            std::cout << emoji << " " << accountNumber << " (" << accountType << ")"
                      << "\n   Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
        }
        
        std::cout << std::string(40, '-') << std::endl;
        std::cout << " Total Portfolio Value: $" << std::fixed << std::setprecision(2) << totalBalance << std::endl;
    }
    
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

std::string BankingSystem::getFullName(const Database::CustomerInfo& customer) const {
    std::string fullName = customer.firstName;
    if (!customer.middleName.empty()) {
        fullName += " " + customer.middleName;
    }
    fullName += " " + customer.lastName;
    return fullName;
}

void BankingSystem::displayTransactionReceipt(const std::string& transactionType, double amount, double newBalance) const {
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "               KNUST BANK" << std::endl;
    std::cout << "           TRANSACTION RECEIPT" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "Account: " << currentAccountNumber << std::endl;
    std::cout << "Transaction: " << transactionType << std::endl;
    std::cout << "Amount: $" << std::fixed << std::setprecision(2) << amount << std::endl;
    std::cout << "New Balance: $" << std::fixed << std::setprecision(2) << newBalance << std::endl;
    
    // Get current time
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::cout << "Date/Time: " << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    std::cout << "Thank you for banking with KNUST Bank!" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
}