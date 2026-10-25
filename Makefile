
# ============================================
# Numerical Recipes Library - Makefile
# ============================================
#
# Environment:
# Windows + MinGW GCC 6.3.0
#
# Commands:
# mingw32-make
# mingw32-make all
# mingw32-make demo
# mingw32-make test
# mingw32-make clean
#
# ============================================

CC      := gcc
CFLAGS  := -std=c11 -O2 -Wall -Wextra -Iinclude
LDFLAGS := -lm

SRC_DIR      := src
EXAMPLES_DIR := examples
TESTS_DIR    := tests
BUILD_DIR    := build
BIN_DIR      := bin

HEADER := include/numerical_recipes.h

# ============================================
# LIBRARY OBJECT FILES
# ============================================

LIB_OBJS := \
    $(BUILD_DIR)/root_finding.o \
    $(BUILD_DIR)/differentiation.o \
    $(BUILD_DIR)/interpolation.o

# ============================================
# DEMO EXECUTABLES
# ============================================

DEMO_ROOT := $(BIN_DIR)/demo_root_finding.exe
DEMO_DIFF := $(BIN_DIR)/demo_differentiation.exe

DEMO_TARGETS := \
    $(DEMO_ROOT) \
    $(DEMO_DIFF)

# ============================================
# TEST EXECUTABLES
# ============================================

TEST_ROOT := $(BIN_DIR)/test_root_finding.exe
TEST_DIFF := $(BIN_DIR)/test_differentiation.exe
TEST_INTERP := $(BIN_DIR)/test_interpolation.exe

TEST_TARGETS := \
    $(TEST_ROOT) \
    $(TEST_DIFF) \
    $(TEST_INTERP)

# ============================================
# MAIN TARGETS
# ============================================

.PHONY: all demo test clean

all: $(LIB_OBJS) $(DEMO_TARGETS) $(TEST_TARGETS)

demo: $(DEMO_TARGETS)

test: $(TEST_TARGETS)
	@echo.
	@echo ====================================
	@echo Running Root Finding Tests
	@echo ====================================
	@$(TEST_ROOT)
	@echo.
	@echo ====================================
	@echo Running Differentiation Tests
	@echo ====================================
	@$(TEST_DIFF)
	@echo.
	@echo ====================================
	@echo Running Interpolation Tests
	@echo ====================================
	@$(TEST_INTERP)

# ============================================
# COMPILE LIBRARY SOURCE FILES
# ============================================

$(BUILD_DIR)/root_finding.o: $(SRC_DIR)/root_finding.c $(HEADER) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/differentiation.o: $(SRC_DIR)/differentiation.c $(HEADER) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/interpolation.o: $(SRC_DIR)/interpolation.c $(HEADER) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ============================================
# COMPILE DEMO SOURCE FILES
# ============================================

$(BUILD_DIR)/demo_root_finding.o: $(EXAMPLES_DIR)/demo_root_finding.c $(HEADER) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/demo_differentiation.o: $(EXAMPLES_DIR)/demo_differentiation.c $(HEADER) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ============================================
# COMPILE TEST SOURCE FILES
# ============================================

$(BUILD_DIR)/test_root_finding.o: $(TESTS_DIR)/test_root_finding.c $(HEADER) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test_differentiation.o: $(TESTS_DIR)/test_differentiation.c $(HEADER) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test_interpolation.o: $(TESTS_DIR)/test_interpolation.c $(HEADER) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# ============================================
# LINK DEMO EXECUTABLES
# ============================================

$(DEMO_ROOT): $(BUILD_DIR)/root_finding.o $(BUILD_DIR)/demo_root_finding.o | $(BIN_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

$(DEMO_DIFF): $(BUILD_DIR)/differentiation.o $(BUILD_DIR)/demo_differentiation.o | $(BIN_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

# ============================================
# LINK TEST EXECUTABLES
# ============================================

$(TEST_ROOT): $(BUILD_DIR)/root_finding.o $(BUILD_DIR)/test_root_finding.o | $(BIN_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

$(TEST_DIFF): $(BUILD_DIR)/differentiation.o $(BUILD_DIR)/test_differentiation.o | $(BIN_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

$(TEST_INTERP): $(BUILD_DIR)/interpolation.o $(BUILD_DIR)/test_interpolation.o | $(BIN_DIR)
	$(CC) $^ $(LDFLAGS) -o $@

# ============================================
# CREATE DIRECTORIES
# ============================================

$(BUILD_DIR) $(BIN_DIR):
	@if not exist "$@" mkdir "$@"

# ============================================
# CLEAN BUILD FILES
# ============================================

clean:
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
	@if exist "$(BIN_DIR)" rmdir /s /q "$(BIN_DIR)"
