# Makefile for Stock Trading Simulator

# Project variables
PROJECT_NAME = stock_trading_simulator
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
OUTPUT_DIR = bin

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
INCLUDES = -I$(INC_DIR) -I$(BUILD_DIR)

# Qt configuration
QT_MODULES = core widgets charts sql network
QT_FLAGS = $(shell pkg-config --cflags $(QT_MODULES))
QT_LIBS = $(shell pkg-config --libs $(QT_MODULES))

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Executable name
TARGET = $(OUTPUT_DIR)/$(PROJECT_NAME)

# Default target
all: $(TARGET)

# Create build and output directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

# Link the executable
$(TARGET): $(OBJECTS) | $(OUTPUT_DIR)
	$(CXX) $(OBJECTS) -o $@ $(QT_LIBS)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(QT_FLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR)/*
	rm -f $(TARGET)

# Rebuild everything
rebuild: clean all

# Phony targets
.PHONY: all clean rebuild

# Dependencies (for development)
dep:
	@echo "Dependencies required:"
	@echo "- Qt5 (with widgets, charts, and sql modules)"
	@echo "- C++17 compatible compiler (gcc or clang)"
	@echo "- pkg-config"
