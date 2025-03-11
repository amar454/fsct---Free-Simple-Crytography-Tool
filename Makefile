# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

# Directories
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Sources
SRC_FILES = $(wildcard $(SRC_DIR)/algorithms/*.cpp) \
			$(wildcard $(SRC_DIR)/client/*.cpp) \
			$(SRC_DIR)/lib/dictionary.cpp

# Objects
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Executable
EXEC = $(BIN_DIR)/cipher_app

# Default target
all: $(EXEC)

# Rule to link object files to create the final executable
$(EXEC): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)  # Ensure the binary directory exists
	$(CXX) $(OBJ_FILES) -o $@

# Rule to compile .cpp files to .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)  # Ensure the object directory exists
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run the program
run: $(EXEC)
	./$(EXEC)

# Rebuild the project
rebuild: clean all

# Declare non-file targets
.PHONY: all clean run rebuild
