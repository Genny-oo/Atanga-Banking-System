#include "BankingSystem.h"
#include "Database.h"
#include <iostream>
#include <string>
#include <memory>

// Function to display system information
void displaySystemInfo() {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "           ATANGA BANKING SYSTEM v2.0" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "\n Secure •  Reliable •  Modern Banking Solution" << std::endl;
    std::cout << "\nFeatures:" << std::endl;
    std::cout << " Multiple Account Types (Savings & Checkings)" << std::endl;
    std::cout << " Secure PIN Authentication" << std::endl;
    std::cout << " Real-time Transaction Processing" << std::endl;
    std::cout << " Complete Transaction History" << std::endl;
    std::cout << " SQLite Database Integration" << std::endl;
    std::cout << " Professional Banking Interface" << std::endl;
    std::cout << "\n Developed by Genevieve Osei-Owusu" << std::endl;
}

int main() {
    try {
        // Display system information
        displaySystemInfo();
        
        std::cout << "\nPress Enter to start the banking system...";
        std::cin.get();
        
        // Create and run the banking system
        std::unique_ptr<BankingSystem> bankingSystem = std::make_unique<BankingSystem>();
        
        std::cout << "\n Starting ATANGA Banking System..." << std::endl;
        std::cout << "Please wait while we initialize the system..." << std::endl;
        
        // Run the main banking system
        bankingSystem->run();
        
    } catch (const std::exception& e) {
        std::cerr << "\n System Error: " << e.what() << std::endl;
        std::cerr << "Please contact system administrator." << std::endl;
        std::cout << "\nPress Enter to exit...";
        std::cin.get();
        return 1;
    } catch (...) {
        std::cerr << "\n Unknown system error occurred." << std::endl;
        std::cerr << "Please contact system administrator." << std::endl;
        std::cout << "\nPress Enter to exit...";
        std::cin.get();
        return 1;
    }
    
    return 0;
}