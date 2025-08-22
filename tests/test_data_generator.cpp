#include "Database.h"
#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

class TestDataGenerator {
private:
    std::unique_ptr<Database> database;
    std::random_device rd;
    std::mt19937 gen;
    
    // Sample data arrays
    std::vector<std::string> firstNames = {
        "Kwame", "Akua", "Kofi", "Ama", "Yaw", "Efua", "Kwaku", "Abena",
        "Kwadwo", "Adwoa", "Kwabena", "Akosua", "Fiifi", "Araba", "Kobby",
        "Maame", "Nana", "Adjoa", "Kojo", "Esi", "Emmanuel", "Grace",
        "Samuel", "Diana", "Michael", "Sarah", "David", "Rebecca"
    };
    
    std::vector<std::string> lastNames = {
        "Asante", "Osei", "Mensah", "Amoah", "Boateng", "Owusu", "Danso",
        "Antwi", "Gyasi", "Nkrumah", "Appiah", "Darko", "Badu", "Frimpong",
        "Opoku", "Yeboah", "Acheampong", "Bonsu", "Marfo", "Wiredu"
    };
    
    std::vector<std::string> cities = {
        "Kumasi", "Accra", "Takoradi", "Tamale", "Cape Coast", "Sunyani",
        "Ho", "Koforidua", "Wa", "Bolgatanga", "Techiman", "Obuasi"
    };
    
    std::vector<std::string> areas = {
        "North Campus", "South Campus", "Unity Hall", "University Hall",
        "Independence Hall", "Republic Hall", "New Hall", "Africa Hall",
        "Ahinsan", "Ayeduase", "Bomso", "Kentinkrono", "Maxima"
    };

public:
    TestDataGenerator() : gen(rd()) {
        database = std::make_unique<Database>();
    }
    
    bool initialize() {
        return database->connect();
    }
    
    std::string generateRandomEmail(const std::string& firstName, const std::string& lastName) {
        std::vector<std::string> domains = {"@gmail.com", "@yahoo.com", "@knust.edu.gh", "@outlook.com"};
        std::uniform_int_distribution<> domainDis(0, domains.size() - 1);
        
        std::string email = firstName + "." + lastName + domains[domainDis(gen)];
        std::transform(email.begin(), email.end(), email.begin(), ::tolower);
        return email;
    }
    
    std::string generateRandomPhone() {
        std::uniform_int_distribution<> dis(20000000, 59999999);
        return "0" + std::to_string(dis(gen));
    }
    
    std::string generateRandomAddress() {
        std::uniform_int_distribution<> cityDis(0, cities.size() - 1);
        std::uniform_int_distribution<> areaDis(0, areas.size() - 1);
        std::uniform_int_distribution<> houseDis(1, 999);
        
        return cities[cityDis(gen)] + ", " + areas[areaDis(gen)] + 
               ", House " + std::to_string(houseDis(gen));
    }
    
    std::string generateRandomDOB() {
        std::uniform_int_distribution<> dayDis(1, 28);
        std::uniform_int_distribution<> monthDis(1, 12);
        std::uniform_int_distribution<> yearDis(1990, 2005);
        
        return std::to_string(dayDis(gen)) + "/" + 
               std::to_string(monthDis(gen)) + "/" + 
               std::to_string(yearDis(gen));
    }
    
    std::string generateRandomPIN() {
        std::uniform_int_distribution<> dis(1000, 9999);
        return std::to_string(dis(gen));
    }
    
    double generateRandomBalance() {
        std::uniform_real_distribution<> dis(100.0, 50000.0);
        return std::round(dis(gen) * 100.0) / 100.0; // Round to 2 decimal places
    }
    
    void generateTestCustomers(int count = 10) {
        std::cout << "🧪 Generating " << count << " test customers..." << std::endl;
        
        std::uniform_int_distribution<> firstNameDis(0, firstNames.size() - 1);
        std::uniform_int_distribution<> lastNameDis(0, lastNames.size() - 1);
        
        for (int i = 0; i < count; ++i) {
            std::string firstName = firstNames[firstNameDis(gen)];
            std::string lastName = lastNames[lastNameDis(gen)];
            std::string middleName = (i % 3 == 0) ? firstNames[firstNameDis(gen)] : "";
            
            std::string email = generateRandomEmail(firstName, lastName);
            std::string phone = generateRandomPhone();
            std::string address = generateRandomAddress();
            std::string dob = generateRandomDOB();
            std::string pin = generateRandomPIN();
            
            if (database->insertCustomer(firstName, middleName, lastName, email, phone, address, dob, pin)) {
                std::cout << "✅ Created customer: " << firstName << " " << lastName 
                         << " (PIN: " << pin << ")" << std::endl;
            } else {
                std::cout << "❌ Failed to create customer: " << firstName << " " << lastName << std::endl;
            }
        }
    }
    
    void generateTestAccountsForCustomers() {
        std::cout << "\n🏦 Creating bank accounts for customers..." << std::endl;
        
        // This is a simplified approach - in a real system you'd query for customer IDs
        std::uniform_int_distribution<> accountTypeDis(0, 1);
        std::vector<std::string> accountTypes = {"Savings", "Checkings"};
        
        for (int customerId = 1; customerId <= 10; ++customerId) {
            // Create 1-2 accounts per customer
            int numAccounts = (customerId % 3 == 0) ? 2 : 1;
            
            for (int j = 0; j < numAccounts; ++j) {
                std::string accountType = accountTypes[accountTypeDis(gen)];
                double initialBalance = generateRandomBalance();
                
                if (database->createAccount(customerId, accountType, initialBalance)) {
                    std::cout << "✅ Created " << accountType << " account for customer " 
                             << customerId << " with balance: $" << std::fixed 
                             << std::setprecision(2) << initialBalance << std::endl;
                }
            }
        }
    }
    
    void generateTestTransactions() {
        std::cout << "\n💳 Generating test transactions..." << std::endl;
        
        // Get all accounts and generate some transactions
        std::uniform_real_distribution<> amountDis(10.0, 1000.0);
        std::uniform_int_distribution<> typeDis(0, 1);
        std::vector<std::string> transactionTypes = {"DEPOSIT", "WITHDRAWAL"};
        
        for (int customerId = 1; customerId <= 10; ++customerId) {
            std::vector<std::string> accounts = database->getCustomerAccounts(customerId);
            
            for (const auto& accountInfo : accounts) {
                size_t pos = accountInfo.find('|');
                std::string accountNumber = accountInfo.substr(0, pos);
                double currentBalance = database->getAccountBalance(accountNumber);
                
                // Generate 3-7 transactions per account
                std::uniform_int_distribution<> transCountDis(3, 7);
                int numTransactions = transCountDis(gen);
                
                for (int k = 0; k < numTransactions; ++k) {
                    std::string transactionType = transactionTypes[typeDis(gen)];
                    double amount = std::round(amountDis(gen) * 100.0) / 100.0;
                    
                    if (transactionType == "WITHDRAWAL" && amount > currentBalance) {
                        // Convert to deposit if insufficient funds
                        transactionType = "DEPOSIT";
                    }
                    
                    double newBalance = (transactionType == "DEPOSIT") ? 
                                      currentBalance + amount : currentBalance - amount;
                    
                    if (newBalance >= 0) {
                        database->updateAccountBalance(accountNumber, newBalance);
                        database->recordTransaction(accountNumber, transactionType, amount, newBalance, 
                                                  "Test " + transactionType.substr(0, 1) + 
                                                  transactionType.substr(1));
                        currentBalance = newBalance;
                    }
                }
                
                std::cout << "✅ Generated transactions for account: " << accountNumber << std::endl;
            }
        }
    }
    
    void displayTestAccounts() {
        std::cout << "\n📋 TEST ACCOUNTS SUMMARY" << std::endl;
        std::cout << std::string(80, '=') << std::endl;
        std::cout << std::left << std::setw(15) << "Customer ID" 
                  << std::setw(12) << "Account#" 
                  << std::setw(12) << "Type"
                  << std::setw(12) << "Balance"
                  << "PIN" << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        for (int customerId = 1; customerId <= 10; ++customerId) {
            Database::CustomerInfo customer = database->getCustomerInfo(customerId);
            if (customer.customerId != -1) {
                std::vector<std::string> accounts = database->getCustomerAccounts(customerId);
                
                for (const auto& accountInfo : accounts) {
                    size_t pos1 = accountInfo.find('|');
                    size_t pos2 = accountInfo.find('|', pos1 + 1);
                    
                    std::string accountNumber = accountInfo.substr(0, pos1);
                    std::string accountType = accountInfo.substr(pos1 + 1, pos2 - pos1 - 1);
                    std::string balance = accountInfo.substr(pos2 + 1);
                    
                    std::cout << std::left << std::setw(15) << customerId
                              << std::setw(12) << accountNumber
                              << std::setw(12) << accountType
                              << "$" << std::setw(11) << std::fixed << std::setprecision(2) << std::stod(balance)
                              << "****" << std::endl; // Don't show actual PINs
                }
            }
        }
        
        std::cout << std::string(80, '=') << std::endl;
        std::cout << "\n💡 Use any account number above with PIN '1234' for testing" << std::endl;
        std::cout << "   (All test accounts have been set with PIN: 1234)" << std::endl;
    }
    
    void generateCompleteTestData() {
        if (!initialize()) {
            std::cerr << "❌ Failed to initialize database!" << std::endl;
            return;
        }
        
        std::cout << "🚀 Generating complete test dataset for KNUST Banking System...\n" << std::endl;
        
        generateTestCustomers(10);
        generateTestAccountsForCustomers();
        generateTestTransactions();
        
        std::cout << "\n✅ Test data generation complete!" << std::endl;
        displayTestAccounts();
        
        std::cout << "\n🎯 Quick Test Instructions:" << std::endl;
        std::cout << "1. Run the banking system: ./banking_system" << std::endl;
        std::cout << "2. Choose 'Login to Existing Account'" << std::endl;
        std::cout << "3. Use any account number from the table above" << std::endl;
        std::cout << "4. Use PIN: 1234 (for all test accounts)" << std::endl;
        std::cout << "5. Explore all banking features!" << std::endl;
    }
};

int main() {
    TestDataGenerator generator;
    
    std::cout << "🏦 KNUST Banking System - Test Data Generator" << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    char choice;
    std::cout << "\nThis will create sample customers, accounts, and transactions." << std::endl;
    std::cout << "⚠️  Warning: This will modify the database!" << std::endl;
    std::cout << "\nProceed? (y/n): ";
    std::cin >> choice;
    
    if (choice == 'y' || choice == 'Y') {
        generator.generateCompleteTestData();
    } else {
        std::cout << "❌ Operation cancelled." << std::endl;
    }
    
    std::cout << "\nPress Enter to exit...";
    std::cin.ignore();
    std::cin.get();
    
    return 0;
}