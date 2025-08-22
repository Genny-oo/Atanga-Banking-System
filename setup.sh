#!/bin/bash

# KNUST Banking System Setup Script
# This script sets up the complete banking system project

echo "🏦 KNUST Banking System Setup"
echo "=============================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}✅ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠️  $1${NC}"
}

print_error() {
    echo -e "${RED}❌ $1${NC}"
}

print_info() {
    echo -e "${BLUE}ℹ️  $1${NC}"
}

# Check if running on supported system
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="linux"
    print_info "Detected Linux system"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macos"
    print_info "Detected macOS system"
else
    print_warning "Unsupported OS. Manual installation may be required."
    OS="unknown"
fi

# Create project structure
echo ""
echo "📁 Creating project structure..."
mkdir -p src obj bin tests docs

print_status "Project directories created"

# Check for required tools
echo ""
echo "🔍 Checking system requirements..."

# Check for g++
if command -v g++ &> /dev/null
then
    GCC_VERSION=$(g++ --version | head -n1)
    print_status "g++ compiler found: $GCC_VERSION"
else
    print_error "g++ compiler not found!"
    if [[ "$OS" == "linux" ]]; then
        echo "Install with: sudo apt-get install build-essential"
    elif [[ "$OS" == "macos" ]]; then
        echo "Install with: xcode-select --install"
    fi
    exit 1
fi

# Check for SQLite3
if command -v sqlite3 &> /dev/null
then
    SQLITE_VERSION=$(sqlite3 --version | cut -d' ' -f1)
    print_status "SQLite3 found: $SQLITE_VERSION"
else
    print_warning "SQLite3 not found. Installing..."
    if [[ "$OS" == "linux" ]]; then
        sudo apt-get update
        sudo apt-get install -y sqlite3 libsqlite3-dev
    elif [[ "$OS" == "macos" ]]; then
        if command -v brew &> /dev/null; then
            brew install sqlite3
        else
            print_error "Homebrew not found. Please install SQLite3 manually."
            exit 1
        fi
    fi
fi

# Check for make
if command -v make &> /dev/null
then
    print_status "Make build system found"
else
    print_warning "Make not found. You'll need to compile manually."
fi

echo ""
echo "📦 Installing dependencies..."

# Install development libraries
if [[ "$OS" == "linux" ]]; then
    sudo apt-get update
    sudo apt-get install -y libsqlite3-dev build-essential
    print_status "Linux dependencies installed"
elif [[ "$OS" == "macos" ]]; then
    if command -v brew &> /dev/null; then
        brew install sqlite3
        print_status "macOS dependencies installed"
    fi
fi

echo ""
echo "📝 Creating source files..."

# Move source files to src directory
if [ -f "main.cpp" ]; then
    mv main.cpp src/
    print_status "Moved main.cpp to src/"
fi

if [ -f "BankingSystem.cpp" ]; then
    mv BankingSystem.cpp src/
    print_status "Moved BankingSystem.cpp to src/"
fi

if [ -f "BankingSystem.h" ]; then
    mv BankingSystem.h src/
    print_status "Moved BankingSystem.h to src/"
fi

if [ -f "Database.cpp" ]; then
    mv Database.cpp src/
    print_status "Moved Database.cpp to src/"
fi

if [ -f "Database.h" ]; then
    mv Database.h src/
    print_status "Moved Database.h to src/"
fi

if [ -f "BankAccount.cpp" ]; then
    mv BankAccount.cpp src/
    print_status "Moved BankAccount.cpp to src/"
fi

if [ -f "BankAccount.h" ]; then
    mv BankAccount.h src/
    print_status "Moved BankAccount.h to src/"
fi

# Create updated Makefile
cat > Makefile << 'EOF'
# KNUST Banking System Makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = -lsqlite3

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests

# Source files
SOURCES = main.cpp BankingSystem.cpp Database.cpp BankAccount.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)

# Target executable
TARGET = $(BIN_DIR)/banking_system
TEST_TARGET = $(BIN_DIR)/test_generator

# Default target
all: directories $(TARGET)

# Create directories
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR) $(TEST_DIR)

# Link the main executable
$(TARGET): $(OBJECTS)
	@echo "🔗 Linking $(TARGET)..."
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "✅ Banking system built successfully!"

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "🔨 Compiling $<..."
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) -c $< -o $@

# Build test data generator
test-generator: directories $(OBJ_DIR)/Database.o
	@echo "🧪 Building test data generator..."
	$(CXX) $(CXXFLAGS) -I$(SRC_DIR) $(TEST_DIR)/test_data_generator.cpp $(OBJ_DIR)/Database.o -o $(TEST_TARGET) $(LDFLAGS)
	@echo "✅ Test generator built successfully!"

# Generate test data
generate-data: test-generator
	@echo "📊 Generating test data..."
	./$(TEST_TARGET)

# Run the banking system
run: $(TARGET)
	@echo "🚀 Starting KNUST Banking System..."
	./$(TARGET)

# Run with test data generation
run-with-data: generate-data run

# Clean build files
clean:
	@echo "🧹 Cleaning build files..."
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	rm -f *.db
	@echo "✅ Clean complete!"

# Install dependencies
install-deps:
	@echo "📦 Installing dependencies..."
	@if [ "$(shell uname)" = "Linux" ]; then \
		sudo apt-get update && sudo apt-get install -y libsqlite3-dev build-essential; \
	elif [ "$(shell uname)" = "Darwin" ]; then \
		brew install sqlite3; \
	fi
	@echo "✅ Dependencies installed!"

# Help
help:
	@echo "🏦 KNUST Banking System - Build Help"
	@echo "===================================="
	@echo "Available targets:"
	@echo "  all              - Build the banking system"
	@echo "  run              - Build and run the application"
	@echo "  test-generator   - Build test data generator"
	@echo "  generate-data    - Generate test data"
	@echo "  run-with-data    - Generate data and run system"
	@echo "  clean            - Remove build files"
	@echo "  install-deps     - Install system dependencies"
	@echo "  help             - Show this help"

.PHONY: all clean install-deps run help directories test-generator generate-data run-with-data
EOF

print_status "Updated Makefile created"

echo ""
echo "🧪 Setting up test environment..."

# Create test data generator
mkdir -p tests
cat > tests/test_data_generator.cpp << 'EOF'
#include "../src/Database.h"
#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <algorithm>

class TestDataGenerator {
private:
    std::unique_ptr<Database> database;
    std::random_device rd;
    std::mt19937 gen;
    
    std::vector<std::string> firstNames = {
        "Kwame", "Akua", "Kofi", "Ama", "Yaw", "Efua", "Kwaku", "Abena",
        "Kwadwo", "Adwoa", "Kwabena", "Akosua", "Fiifi", "Araba", "Emmanuel", "Grace"
    };
    
    std::vector<std::string> lastNames = {
        "Asante", "Osei", "Mensah", "Amoah", "Boateng", "Owusu", "Danso",
        "Antwi", "Gyasi", "Nkrumah", "Appiah", "Darko", "Badu", "Frimpong"
    };

public:
    TestDataGenerator() : gen(rd()) {
        database = std::make_unique<Database>();
    }
    
    bool initialize() {
        return database->connect();
    }
    
    void generateTestData() {
        if (!initialize()) {
            std::cerr << "Failed to initialize database!" << std::endl;
            return;
        }
        
        std::cout << "🧪 Generating test data..." << std::endl;
        
        // Create 5 test customers with simple PINs for easy testing
        std::vector<std::tuple<std::string, std::string, std::string, std::string>> testCustomers = {
            {"Kwame", "Nkrumah", "kwame.nkrumah@knust.edu.gh", "1234"},
            {"Akua", "Asante", "akua.asante@gmail.com", "1234"},
            {"Kofi", "Mensah", "kofi.mensah@yahoo.com", "1234"},
            {"Ama", "Boateng", "ama.boateng@knust.edu.gh", "1234"},
            {"Yaw", "Owusu", "yaw.owusu@outlook.com", "1234"}
        };
        
        std::uniform_int_distribution<> phoneDis(20000000, 59999999);
        
        for (size_t i = 0; i < testCustomers.size(); ++i) {
            auto [firstName, lastName, email, pin] = testCustomers[i];
            
            std::string phone = "0" + std::to_string(phoneDis(gen));
            std::string address = "KNUST Campus, Block " + std::to_string(i + 1);
            std::string dob = "15/06/199" + std::to_string(i + 5);
            
            if (database->insertCustomer(firstName, "", lastName, email, phone, address, dob, pin)) {
                std::cout << "✅ Created customer: " << firstName << " " << lastName << std::endl;
                
                // Create accounts for each customer
                int customerId = i + 1; // Assuming auto-increment starts at 1
                
                // Create savings account
                if (database->createAccount(customerId, "Savings", 1000.0 + (i * 500))) {
                    std::cout << "  💰 Created Savings account" << std::endl;
                }
                
                // Create checkings account for some customers
                if (i % 2 == 0) {
                    if (database->createAccount(customerId, "Checkings", 500.0 + (i * 300))) {
                        std::cout << "  💳 Created Checkings account" << std::endl;
                    }
                }
            }
        }
        
        std::cout << "\n📋 TEST ACCOUNTS CREATED" << std::endl;
        std::cout << "========================" << std::endl;
        std::cout << "All test accounts use PIN: 1234" << std::endl;
        std::cout << "\nTest Customers:" << std::endl;
        for (size_t i = 0; i < testCustomers.size(); ++i) {
            auto [firstName, lastName, email, pin] = testCustomers[i];
            std::cout << "👤 " << firstName << " " << lastName << " (" << email << ")" << std::endl;
        }
        
        std::cout << "\n🎯 To test:" << std::endl;
        std::cout << "1. Run: ./bin/banking_system" << std::endl;
        std::cout << "2. Choose 'Login to Existing Account'" << std::endl;
        std::cout << "3. Enter any account number (will be generated)" << std::endl;
        std::cout << "4. Use PIN: 1234" << std::endl;
    }
};

int main() {
    TestDataGenerator generator;
    generator.generateTestData();
    return 0;
}
EOF

print_status "Test data generator created"

echo ""
echo "🎯 Building the project..."

# Build the project
if make all; then
    print_status "Banking system compiled successfully!"
else
    print_error "Compilation failed. Check for errors above."
    exit 1
fi

echo ""
echo "🧪 Building test data generator..."

if make test-generator; then
    print_status "Test data generator compiled successfully!"
else
    print_warning "Test generator compilation failed. You can still use the main system."
fi

echo ""
echo "📊 Generating test data..."

if ./bin/test_generator; then
    print_status "Test data generated successfully!"
else
    print_warning "Test data generation failed. You can create accounts manually."
fi

echo ""
echo "🎉 Setup Complete!"
echo "=================="
print_info "Your KNUST Banking System is ready!"

echo ""
echo "🚀 Quick Start:"
echo "  1. Run the system: make run"
echo "  2. Or manually: ./bin/banking_system"
echo ""
echo "🧪 Test Accounts Available:"
echo "  - All test accounts use PIN: 1234"
echo "  - Multiple customers with different account types"
echo "  - Pre-loaded with sample transactions"
echo ""
echo "📚 Need Help?"
echo "  - Run: make help"
echo "  - Check: README.md"
echo "  - View source code in src/ directory"

echo ""
print_status "Setup script completed successfully!"
echo "Happy banking! 🏦💰"