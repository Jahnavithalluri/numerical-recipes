# Makefile for the Numerical Recipes root-finding demo and tests.
#
# Environment: MinGW GCC 6.3.0 on Windows. `mingw32-make` is available;
# plain `make` is not. Recipes are written for cmd.exe (mingw32-make's
# default shell on MinGW.org) and avoid Unix-only commands such as `true`.

CC      := gcc
CFLAGS  := -std=c11 -O2 -Wall -Wextra -Iinclude
LDFLAGS := -lm

SRC_DIR     := src
EXAMPLES_DIR := examples
TESTS_DIR   := tests
BUILD_DIR   := build
BIN_DIR     := bin

DEMO_TARGET      := $(BIN_DIR)/demo_root_finding.exe
TEST_TARGET      := $(BIN_DIR)/test_root_finding.exe

DEMO_OBJS := $(BUILD_DIR)/root_finding.o $(BUILD_DIR)/demo_root_finding.o
TEST_OBJS := $(BUILD_DIR)/root_finding.o $(BUILD_DIR)/test_root_finding.o

.PHONY: all clean test demo

all: $(DEMO_TARGET) $(TEST_TARGET)

demo: $(DEMO_TARGET)

test: $(TEST_TARGET)
	@$(TEST_TARGET)

$(DEMO_TARGET): $(DEMO_OBJS) | $(BIN_DIR)
	$(CC) $(DEMO_OBJS) $(LDFLAGS) -o $@

$(TEST_TARGET): $(TEST_OBJS) | $(BIN_DIR)
	$(CC) $(TEST_OBJS) $(LDFLAGS) -o $@

$(BUILD_DIR)/root_finding.o: $(SRC_DIR)/root_finding.c include/numerical_recipes.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/demo_root_finding.o: $(EXAMPLES_DIR)/demo_root_finding.c include/numerical_recipes.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test_root_finding.o: $(TESTS_DIR)/test_root_finding.c include/numerical_recipes.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR) $(BIN_DIR):
	@if not exist "$@" mkdir "$@"

clean:
	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
	@if exist "$(BIN_DIR)" rmdir /s /q "$(BIN_DIR)"
