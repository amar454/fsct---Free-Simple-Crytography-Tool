# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude
LDFLAGS = -lcurl

# Directories
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
CIPHERS_OBJ_DIR = $(OBJ_DIR)/ciphers
CLIENT_OBJ_DIR = $(OBJ_DIR)/client
DICTIONARY_OBJ_DIR = $(OBJ_DIR)/dictionary

# Sources
SRC_FILES = $(wildcard $(SRC_DIR)/ciphers/*.cpp) \
            $(wildcard $(SRC_DIR)/client/*.cpp) \
            $(wildcard $(SRC_DIR)/dictionary/*.cpp) \
            $(wildcard $(SRC_DIR)/analysis/*.cpp) \
            $(wildcard $(SRC_DIR)/suggestions/*.cpp) \
            $(wildcard $(SRC_DIR)/formatting/*.cpp)

# Objects
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Executable
EXEC = $(BIN_DIR)/fsct

# Default target
all: $(EXEC)

# Rule to link object files to create the final executable
$(EXEC): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)

# Rule to compile .cpp files to .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run the program
run: $(EXEC)
	./$(EXEC)

test:
	./test.sh

# Rebuild the project
rebuild: clean all

# Declare non-file targets
.PHONY: all clean run rebuild test
