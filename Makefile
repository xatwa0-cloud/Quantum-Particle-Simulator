#**************************************************************************************************
#
#   
#   If Schrödinger’s cat sees this, it both approves and disapproves
#	Just like this code before compilation
#  
#
#**************************************************************************************************

.PHONY: all clean run debug help

# ===========================================================================================
# Compilation Configuration
# ===========================================================================================

PROJECT_NAME = quantum

# C++ compiler and flags
CXX = g++

# Common warnings / standard flags
CXXFLAGS_COMMON = -std=c++14 -Wall -Wextra -Wno-missing-field-initializers

# Build-type specific flags
CXXFLAGS_DEBUG   = $(CXXFLAGS_COMMON) -g -O0
CXXFLAGS_RELEASE = $(CXXFLAGS_COMMON) -O2

# By default, use release-style flags; can be overridden:
#   make CXX=clang++ CXXFLAGS="$(CXXFLAGS_DEBUG)"
CXXFLAGS ?= $(CXXFLAGS_RELEASE)

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = .

# Output
EXECUTABLE = $(BIN_DIR)/$(PROJECT_NAME).exe

# ===========================================================================================
# Source Files & Object Files
# ===========================================================================================

# Automatically pick up all .cpp files under src/
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

# ===========================================================================================
# Include & Lib Paths 
# ===========================================================================================

# Location of the raylib install; override if needed, e.g.:
#   make RAYLIB_DIR=D:/dev/raylib/raylib/src
RAYLIB_DIR ?= C:/raylib/raylib/src

INCLUDES = -I$(INC_DIR) -I. -I"$(RAYLIB_DIR)"
LIBS = -L"$(RAYLIB_DIR)" -lraylib -lopengl32 -lgdi32 -lwinmm

# ===========================================================================================
# Build Targets
# ===========================================================================================

all: $(EXECUTABLE)
	@echo.
	@echo ========================================
	@echo   Build Complete: $(EXECUTABLE)
	@echo ========================================
	@echo.

$(EXECUTABLE): $(OBJECTS)
	@echo.
	@echo [LINKING] Building $(EXECUTABLE)...
	@echo.
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	@echo [COMPILING] $<
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ===========================================================================================
# Utility Targets
# ===========================================================================================

run: $(EXECUTABLE)
	@echo.
	@echo [RUNNING] Launching $(EXECUTABLE)...
	@echo.
	$(EXECUTABLE)

clean:
	@echo.
	@echo [CLEANING] Removing build artifacts...
	@if exist $(OBJ_DIR) rmdir /s /q $(OBJ_DIR)
	@if exist $(EXECUTABLE) del /q $(EXECUTABLE)
	@echo [DONE] Build artifacts removed
	@echo.

debug:
	@echo.
	@echo [DEBUG] Building debug configuration...
	@echo.
	$(MAKE) clean
	$(MAKE) CXXFLAGS="$(CXXFLAGS_DEBUG)"
	@echo.
	@echo [DEBUG] Debug build complete with symbols
	@echo.

help:
	@echo.
	@echo ==========================================
	@echo Quantum Particle Simulator - Build System
	@echo ==========================================
	@echo.
	@echo Available targets:
	@echo   make all       - Build the project (default^)
	@echo   make run       - Build and run the executable
	@echo   make clean     - Remove build artifacts
	@echo   make debug     - Build with debug symbols
	@echo   make help      - Show this help message
	@echo.
	@echo Project structure:
	@echo   src/           - Source files (.cpp^)
	@echo   include/       - Header files (.h^)
	@echo   obj/           - Object files (generated^)
	@echo.

.DEFAULT_GOAL := all
