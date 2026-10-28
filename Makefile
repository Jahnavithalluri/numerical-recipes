# ============================================================
# Numerical Recipes Library - GNU Make build
# Primary environment: Windows + MinGW GCC 6.3.0
# Also supports GNU Make + GCC on Linux for reproducibility.
# ============================================================

CC       := gcc
AR       := ar
ARFLAGS  := rcs
CFLAGS   := -std=c11 -O2 -Wall -Wextra -Iinclude
LDLIBS   := -lm

SRC_DIR      := src
EXAMPLES_DIR := examples
TESTS_DIR    := tests
BUILD_DIR    := build
BIN_DIR      := bin
LIB_DIR      := lib
HEADER       := include/numerical_recipes.h
STATIC_LIB   := $(LIB_DIR)/libnumerical_recipes.a

# Windows Command Prompt and POSIX shells need different directory commands.
ifeq ($(OS),Windows_NT)
EXE := .exe
MKDIR = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
REMOVE_DIR = if exist "$(subst /,\,$(1))" rmdir /s /q "$(subst /,\,$(1))"
else
EXE :=
MKDIR = mkdir -p "$(1)"
REMOVE_DIR = rm -rf "$(1)"
endif

LIB_OBJS := \
    $(BUILD_DIR)/root_finding.o \
    $(BUILD_DIR)/differentiation.o \
    $(BUILD_DIR)/interpolation.o

DEMO_ROOT   := $(BIN_DIR)/demo_root_finding$(EXE)
DEMO_DIFF   := $(BIN_DIR)/demo_differentiation$(EXE)
DEMO_INTERP := $(BIN_DIR)/demo_interpolation$(EXE)
DEMO_TARGETS := $(DEMO_ROOT) $(DEMO_DIFF) $(DEMO_INTERP)

TEST_ROOT   := $(BIN_DIR)/test_root_finding$(EXE)
TEST_DIFF   := $(BIN_DIR)/test_differentiation$(EXE)
TEST_INTERP := $(BIN_DIR)/test_interpolation$(EXE)
TEST_ROBUST := $(BIN_DIR)/test_robustness$(EXE)
TEST_TARGETS := $(TEST_ROOT) $(TEST_DIFF) $(TEST_INTERP) $(TEST_ROBUST)

.PHONY: all library demo test clean

all: library $(DEMO_TARGETS) $(TEST_TARGETS)
library: $(STATIC_LIB)
demo: $(DEMO_TARGETS)

test: $(TEST_TARGETS)
	@echo Running Root Finding Tests
	@$(TEST_ROOT)
	@echo Running Differentiation Tests
	@$(TEST_DIFF)
	@echo Running Interpolation Tests
	@$(TEST_INTERP)
	@echo Running Robustness Tests
	@$(TEST_ROBUST)

# A real reusable static library, containing only the library modules.
$(STATIC_LIB): $(LIB_OBJS) | $(LIB_DIR)
	$(AR) $(ARFLAGS) $@ $(LIB_OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/demo_%.o: $(EXAMPLES_DIR)/demo_%.c $(HEADER) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test_%.o: $(TESTS_DIR)/test_%.c $(HEADER) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link applications against the archive, not against src/*.o directly.
# -lm MUST follow the archive so the math symbols can be resolved.
$(BIN_DIR)/demo_%$(EXE): $(BUILD_DIR)/demo_%.o $(STATIC_LIB) | $(BIN_DIR)
	$(CC) $< $(STATIC_LIB) $(LDLIBS) -o $@

$(BIN_DIR)/test_%$(EXE): $(BUILD_DIR)/test_%.o $(STATIC_LIB) | $(BIN_DIR)
	$(CC) $< $(STATIC_LIB) $(LDLIBS) -o $@

$(BUILD_DIR) $(BIN_DIR) $(LIB_DIR):
	@$(call MKDIR,$@)

clean:
	@$(call REMOVE_DIR,$(BUILD_DIR))
	@$(call REMOVE_DIR,$(BIN_DIR))
	@$(call REMOVE_DIR,$(LIB_DIR))
