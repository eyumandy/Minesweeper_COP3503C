# Default target
.PHONY: all clean

# Variables
CMAKE_COMMAND = /usr/bin/cmake
CMAKE_SOURCE_DIR = .
CMAKE_BINARY_DIR = ./build

# Build all
all: $(CMAKE_BINARY_DIR)/minesweeper

# Clean build
clean:
	@rm -rf $(CMAKE_BINARY_DIR)

# Build minesweeper
$(CMAKE_BINARY_DIR)/minesweeper: $(CMAKE_BINARY_DIR)/CMakeFiles/Makefile.cmake
	@$(MAKE) -C $(CMAKE_BINARY_DIR)
	@cp $(CMAKE_BINARY_DIR)/minesweeper .

# Ensure build directory exists and configure it
$(CMAKE_BINARY_DIR)/CMakeFiles/Makefile.cmake:
	@mkdir -p $(CMAKE_BINARY_DIR)
	@$(CMAKE_COMMAND) -S $(CMAKE_SOURCE_DIR) -B $(CMAKE_BINARY_DIR)