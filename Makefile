# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -I./src -Wall -Wextra
LDFLAGS = -lsqlite3

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests

# Target executable
TARGET = $(BIN_DIR)/banking_system.exe
TEST_TARGET = $(BIN_DIR)/test_data_generator.exe

# Source files
SRCS = $(SRC_DIR)/main.cpp \
       $(SRC_DIR)/BankingSystem.cpp \
       $(SRC_DIR)/BankAccount.cpp \
       $(SRC_DIR)/Database.cpp

OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Test source files
TEST_SRCS = $(TEST_DIR)/test_data_generator.cpp \
            $(SRC_DIR)/BankingSystem.cpp \
            $(SRC_DIR)/BankAccount.cpp \
            $(SRC_DIR)/Database.cpp

TEST_OBJS = $(TEST_SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TEST_OBJS := $(TEST_OBJS:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Default target
all: $(TARGET) $(TEST_TARGET)

# Build main program
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# Build test program
$(TEST_TARGET): $(TEST_OBJS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_OBJS) $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories if not exist
$(BIN_DIR) $(OBJ_DIR):
	mkdir $@

# Clean build files
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*.exe

.PHONY: all clean
