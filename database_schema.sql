-- KNUST Banking System Database Schema
-- SQLite Database Structure

-- ============================================
-- CUSTOMERS TABLE
-- Stores customer personal information
-- ============================================
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

-- ============================================
-- ACCOUNTS TABLE  
-- Stores bank account information
-- ============================================
CREATE TABLE IF NOT EXISTS accounts (
    account_number TEXT PRIMARY KEY,
    customer_id INTEGER NOT NULL,
    account_type TEXT NOT NULL CHECK (account_type IN ('Savings', 'Checkings', 'Current', 'Business')),
    balance REAL NOT NULL DEFAULT 0.0 CHECK (balance >= 0),
    status TEXT DEFAULT 'ACTIVE' CHECK (status IN ('ACTIVE', 'INACTIVE', 'FROZEN', 'CLOSED')),
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (customer_id) REFERENCES customers (customer_id) ON DELETE CASCADE
);

-- ============================================
-- TRANSACTIONS TABLE
-- Stores all banking transactions
-- ============================================
CREATE TABLE IF NOT EXISTS transactions (
    transaction_id INTEGER PRIMARY KEY AUTOINCREMENT,
    account_number TEXT NOT NULL,
    transaction_type TEXT NOT NULL CHECK (transaction_type IN ('DEPOSIT', 'WITHDRAWAL', 'TRANSFER_IN', 'TRANSFER_OUT', 'INTEREST', 'FEE')),
    amount REAL NOT NULL CHECK (amount > 0),
    balance_after REAL NOT NULL CHECK (balance_after >= 0),
    description TEXT,
    transaction_date DATETIME DEFAULT CURRENT_TIMESTAMP,
    reference_number TEXT,
    FOREIGN KEY (account_number) REFERENCES accounts (account_number) ON DELETE CASCADE
);

-- ============================================
-- INDEXES FOR PERFORMANCE
-- ============================================
CREATE INDEX IF NOT EXISTS idx_customers_email ON customers(email);
CREATE INDEX IF NOT EXISTS idx_accounts_customer_id ON accounts(customer_id);
CREATE INDEX IF NOT EXISTS idx_accounts_status ON accounts(status);
CREATE INDEX IF NOT EXISTS idx_transactions_account_number ON transactions(account_number);
CREATE INDEX IF NOT EXISTS idx_transactions_date ON transactions(transaction_date);
CREATE INDEX IF NOT EXISTS idx_transactions_type ON transactions(transaction_type);

-- ============================================
-- SAMPLE TEST DATA
-- ============================================

-- Insert sample customers
INSERT OR IGNORE INTO customers (first_name, middle_name, last_name, email, phone_number, address, date_of_birth, pin) VALUES
('Kwame', 'Nkrumah', 'Asante', 'kwame.asante@knust.edu.gh', '0244123456', 'KNUST Campus, Unity Hall', '15/06/1995', '1234'),
('Akua', 'Serwaa', 'Mensah', 'akua.mensah@gmail.com', '0207654321', 'Kumasi, Ahinsan Estate', '22/03/1998', '1234'),
('Kofi', 'Kwaku', 'Osei', 'kofi.osei@yahoo.com', '0556789123', 'Accra, East Legon', '10/11/1996', '1234'),
('Ama', 'Adjoa', 'Boateng', 'ama.boateng@knust.edu.gh', '0274567890', 'KNUST Campus, Republic Hall', '05/09/1997', '1234'),
('Yaw', 'Kwame', 'Owusu', 'yaw.owusu@outlook.com', '0501234567', 'Takoradi, New Takoradi', '18/12/1999', '1234');

-- Insert sample accounts
INSERT OR IGNORE INTO accounts (account_number, customer_id, account_type, balance, status) VALUES
('100000001', 1, 'Savings', 5000.00, 'ACTIVE'),
('100000002', 1, 'Checkings', 2500.00, 'ACTIVE'),
('100000003', 2, 'Savings', 3200.00, 'ACTIVE'),
('100000004', 3, 'Checkings', 1800.00, 'ACTIVE'),
('100000005', 3, 'Savings', 7500.00, 'ACTIVE'),
('100000006', 4, 'Savings', 4200.00, 'ACTIVE'),
('100000007', 5, 'Checkings', 950.00, 'ACTIVE');

-- Insert sample transactions
INSERT OR IGNORE INTO transactions (account_number, transaction_type, amount, balance_after, description) VALUES
('100000001', 'DEPOSIT', 5000.00, 5000.00, 'Initial deposit'),
('100000001', 'WITHDRAWAL', 500.00, 4500.00, 'ATM withdrawal'),
('100000001', 'DEPOSIT', 1200.00, 5700.00, 'Salary deposit'),
('100000001', 'WITHDRAWAL', 700.00, 5000.00, 'Cash withdrawal'),

('100000002', 'DEPOSIT', 2500.00, 2500.00, 'Initial deposit'),
('100000002', 'WITHDRAWAL', 150.00, 2350.00, 'ATM withdrawal'),
('100000002', 'WITHDRAWAL', 200.00, 2150.00, 'Cash withdrawal'),
('100000002', 'DEPOSIT', 350.00, 2500.00, 'Cash deposit'),

('100000003', 'DEPOSIT', 3200.00, 3200.00, 'Initial deposit'),
('100000003', 'DEPOSIT', 800.00, 4000.00, 'Monthly savings'),
('100000003', 'WITHDRAWAL', 800.00, 3200.00, 'Emergency withdrawal'),

('100000004', 'DEPOSIT', 1800.00, 1800.00, 'Initial deposit'),
('100000004', 'WITHDRAWAL', 300.00, 1500.00, 'Bill payment'),
('100000004', 'DEPOSIT', 500.00, 2000.00, 'Freelance payment'),
('100000004', 'WITHDRAWAL', 200.00, 1800.00, 'Shopping'),

('100000005', 'DEPOSIT', 7500.00, 7500.00, 'Initial deposit'),

('100000006', 'DEPOSIT', 4200.00, 4200.00, 'Initial deposit'),
('100000006', 'WITHDRAWAL', 400.00, 3800.00, 'School fees'),
('100000006', 'DEPOSIT', 400.00, 4200.00, 'Scholarship refund'),

('100000007', 'DEPOSIT', 950.00, 950.00, 'Initial deposit');

-- ============================================
-- USEFUL QUERIES FOR TESTING
-- ============================================

-- View all customers
-- SELECT * FROM customers;

-- View all accounts with customer names
-- SELECT 
--     a.account_number,
--     c.first_name || ' ' || c.last_name as customer_name,
--     a.account_type,
--     a.balance,
--     a.status
-- FROM accounts a
-- JOIN customers c ON a.customer_id = c.customer_id;

-- View transaction history for an account
-- SELECT 
--     transaction_type,
--     amount,
--     balance_after,
--     description,
--     transaction_date
-- FROM transactions 
-- WHERE account_number = '100000001'
-- ORDER BY transaction_date DESC;

-- Customer account summary
-- SELECT 
--     c.first_name || ' ' || c.last_name as customer_name,
--     COUNT(a.account_number) as total_accounts,
--     SUM(a.balance) as total_balance
-- FROM customers c
-- LEFT JOIN accounts a ON c.customer_id = a.customer_id
-- WHERE a.status = 'ACTIVE'
-- GROUP BY c.customer_id;