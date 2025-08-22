# 🏦 ATANGA BANKING SYSTEM

**A Modern Banking Management System**  
*By Genevieve Osei-Owusu*  


## 📋 Overview

The Atanga Banking System is a comprehensive banking management application built in C++ with SQLite database integration. It simulates real banking operations including account management, transactions, and customer services.

## ✨ Features

### 🔐 Security Features
- ✅ 4-digit PIN authentication
- ✅ Secure customer login system
- ✅ Account validation and verification
- ✅ Transaction authorization

### 🏦 Banking Operations
- ✅ **Multiple Account Types**: Savings and Checkings accounts
- ✅ **Deposit & Withdrawal**: Real-time balance updates
- ✅ **Balance Inquiry**: Instant balance checking
- ✅ **Transaction History**: Complete transaction records
- ✅ **Account Management**: Multiple accounts per customer
- ✅ **Customer Registration**: New customer onboarding

### 💾 Database Features
- ✅ **SQLite Integration**: Persistent data storage
- ✅ **Relational Design**: Customers, Accounts, and Transactions tables
- ✅ **Data Integrity**: Foreign key constraints and validation
- ✅ **Transaction Logging**: Complete audit trail

## 🏗️ Architecture

### File Structure
```
KNUST-Banking-System/
├── 📁 src/
│   ├── 📄 main.cpp                 # Main entry point
│   ├── 📄 BankingSystem.h/.cpp     # Core banking system
│   ├── 📄 Database.h/.cpp          # Database operations
│   └── 📄 BankAccount.h/.cpp       # Account management
├── 📁 tests/
│   └── 📄 test_data_generator.cpp  # Test data creation
├── 📄 Makefile                     # Build configuration
├── 📄 README.md                    # This file
└── 📄 bank_system.db              # SQLite database (auto-created)
|___📁 sqlite/                     #SQLite amalgamation
```

### Database Schema

#### Customers Table
| Field | Type | Description |
|-------|------|-------------|
| customer_id | INTEGER PRIMARY KEY | Unique customer identifier |
| first_name | TEXT | Customer's first name |
| middle_name | TEXT | Customer's middle name (optional) |
| last_name | TEXT | Customer's last name |
| email | TEXT UNIQUE | Customer's email address |
| phone_number | TEXT | Customer's phone number |
| address | TEXT | Customer's complete address |
| date_of_birth | TEXT | Customer's date of birth |
| pin | TEXT | 4-digit security PIN |
| created_at | DATETIME | Account creation timestamp |

#### Accounts Table
| Field | Type | Description |
|-------|------|-------------|
| account_number | TEXT PRIMARY KEY | Unique 9-digit account number |
| customer_id | INTEGER | Foreign key to customers table |
| account_type | TEXT | Account type (Savings/Checkings) |
| balance | REAL | Current account balance |
| status | TEXT | Account status (ACTIVE/INACTIVE/FROZEN) |
| created_at | DATETIME | Account creation timestamp |

#### Transactions Table
| Field | Type | Description |
|-------|------|-------------|
| transaction_id | INTEGER PRIMARY KEY | Unique transaction identifier |
| account_number | TEXT | Foreign key to accounts table |
| transaction_type | TEXT | Transaction type (DEPOSIT/WITHDRAWAL) |
| amount | REAL | Transaction amount |
| balance_after | REAL | Account balance after transaction |
| description | TEXT | Transaction description |
| transaction_date | DATETIME | Transaction timestamp |

## 🚀 Installation & Setup

### Prerequisites
- **C++ Compiler**: GCC or Clang 
- **SQLite3**: Development libraries
- **Make**: Build system

### Ubuntu/Debian Installation
```bash
# Install dependencies
sudo apt-get update
sudo apt-get install build-essential libsqlite3-dev

# Or use the Makefile
make install-deps
```

### macOS Installation
```bash
# Using Homebrew
brew install sqlite3

# Using MacPorts
sudo port install sqlite3
```

### Windows Installation
1. Install MinGW-w64 or Visual Studio
2. Download SQLite3 development libraries
3. Configure include and library paths

## 🔨 Building the Project

### Using Makefile (Recommended)
```bash
# Build the project
make

# Build and run
make run

# Clean build files
make clean
```

### Manual Compilation
```bash
# Compile all source files
g++ -I./sqlite/sqlite-amalgamation-3500400 src/main.cpp src/BankingSystem.cpp src/Database.cpp src/BankAccount.cpp sqlite3.o -o myprogram.exe

# Run the application
.\myprogram.exe
```

## 🧪 Testing

### Generate Test Data
```bash
# Compile test data generator
g++ -std=c++17 tests/test_data_generator.cpp src/Database.cpp sqlite3.o -o test_generator.exe

# Generate test data
./test_generator.exe
```

### Test Accounts
After running the test data generator, you can use these sample credentials:

| Customer Name | Account Type | PIN | Notes |
|---------------|--------------|-----|-------|
| Kwame Asante | Savings | 1234 | High balance |
| Akua Mensah | Checkings | 1234 | Multiple transactions |
| Kofi Osei | Both | 1234 | Multiple accounts |

*All test accounts use PIN: **1234***

## 🎮 Usage Guide

### 1. Starting the System
```bash
.\myprogram.exe
```

### 2. Creating a New Customer
1. Select "Create New Customer Account"
2. Fill in personal information
3. Create a 4-digit PIN
4. Optionally open a bank account

### 3. Logging In
1. Select "Login to Existing Account"
2. Enter your account number
3. Enter your 4-digit PIN

### 4. Banking Operations
- **💰 Check Balance**: View current account balance
- **📥 Deposit**: Add money to your account
- **📤 Withdraw**: Remove money from your account
- **📊 Transaction History**: View recent transactions
- **🔄 Switch Account**: Change active account
- **ℹ️ Account Info**: View complete account details

## 💡 Key Features Explained

### Transaction Limits
- **Minimum Transaction**: $1.00
- **Maximum Withdrawal**: $10,000.00 per transaction
- **Daily Withdrawal Limit**: $50,000.00

### Security Features
- 4-digit PIN authentication
- Account number validation
- Transaction authorization
- Secure database storage

### User Experience
- Clear, emoji-enhanced interface
- Real-time balance updates
- Transaction receipts
- Error handling and validation

## 🔧 Advanced Configuration

### Database Configuration
The system uses SQLite by default. To use a different database location:

```cpp
Database db("custom_path/bank_system.db");
```

### Transaction Limits
Modify limits in `BankingSystem.h`:

```cpp
static const double MAX_WITHDRAWAL_AMOUNT = 10000.0;
static const double MIN_TRANSACTION_AMOUNT = 1.0;
static const double MAX_DAILY_WITHDRAWAL = 50000.0;
```

## 🐛 Troubleshooting

### Common Issues

**Database Connection Failed**
```bash
# Check SQLite3 installation
sqlite3 --version

# Verify permissions
ls -la bank_system.db
```

**Compilation Errors**
```bash
# Check compiler version
g++ --version

# Verify SQLite3 development libraries
dpkg -l | grep sqlite3-dev  # Ubuntu/Debian
brew list | grep sqlite     # macOS
```

**Runtime Errors**
- Ensure database file permissions are correct
- Check that SQLite3 is properly installed
- Verify all source files are present

## 🚀 Future Enhancements
### Planned Features
- 🔄 **Fund Transfers**: Between accounts and customers
- 📱 **Mobile Banking**: Mobile-friendly interface
- 💳 **Debit Card**: Virtual card management
- 📈 **Interest Calculation**: Automatic interest on savings
- 🔔 **Notifications**: SMS and email alerts
- 🌐 **Web Interface**: Browser-based banking
- 📊 **Reports**: Monthly statements and analytics

### Technical Improvements
- **Multi-threading**: Concurrent transaction processing
- **Encryption**: Enhanced data security
- **API Integration**: External service connections
- **Cloud Database**: Remote database support
- **Backup System**: Automated data backups



**🎯 Ready to start banking?**  
Run `make run` and experience modern banking with KNUST Bank! 🏦