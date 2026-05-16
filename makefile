# makefile for companders library (integer companding for embedded systems)
# @author M A Chatterjee <deftio [at] deftio [dot] com>

CC        = gcc
CFLAGS    = -I./src -Wall
CFLAGS_COV= -I./src -Wall -ftest-coverage -fprofile-arcs

SRC_DIR      = src
TEST_DIR     = test
EXAMPLES_DIR = examples
BIN_DIR      = bin

# Source files
LIB_SRC   = $(SRC_DIR)/companders.c

# Build outputs (all in bin/)
LIB_OBJ       = $(BIN_DIR)/companders.o
LIB_OBJ_PLAIN = $(BIN_DIR)/companders_plain.o

TEST_OBJ  = $(BIN_DIR)/companders_fulltest.o
TEST_BIN  = $(BIN_DIR)/companders_fulltest

EXAMPLE_OBJ = $(BIN_DIR)/compandit.o
EXAMPLE_BIN = $(BIN_DIR)/compandit

# Default target: print available commands
.PHONY: help
help:
	@echo ""
	@echo "companders - integer companding library for embedded systems"
	@echo "============================================================"
	@echo ""
	@echo "Available commands:"
	@echo "  make build      - Build the example program"
	@echo "  make test       - Build and run the test suite"
	@echo "  make coverage   - Build, test, and generate coverage report"
	@echo "  make clean      - Remove build artifacts"
	@echo "  make help       - Show this help message"
	@echo ""

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Build the example program (no coverage instrumentation)
.PHONY: build
build: $(EXAMPLE_BIN)

$(LIB_OBJ_PLAIN): $(LIB_SRC) $(SRC_DIR)/companders.h | $(BIN_DIR)
	$(CC) -c -o $@ $(LIB_SRC) $(CFLAGS)

$(EXAMPLE_OBJ): $(EXAMPLES_DIR)/compandit.c $(SRC_DIR)/companders.h | $(BIN_DIR)
	$(CC) -c -o $@ $(EXAMPLES_DIR)/compandit.c $(CFLAGS)

$(EXAMPLE_BIN): $(LIB_OBJ_PLAIN) $(EXAMPLE_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -lm

# Compile object files with coverage flags
$(LIB_OBJ): $(LIB_SRC) $(SRC_DIR)/companders.h | $(BIN_DIR)
	$(CC) -c -o $@ $(LIB_SRC) $(CFLAGS_COV)

$(TEST_OBJ): $(TEST_DIR)/companders_fulltest.c $(SRC_DIR)/companders.h | $(BIN_DIR)
	$(CC) -c -o $@ $(TEST_DIR)/companders_fulltest.c $(CFLAGS_COV)

# Build and run tests
.PHONY: test
test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(LIB_OBJ) $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS_COV) -lm

# Coverage: build with coverage flags, run tests, generate report
.PHONY: coverage
coverage: test
	lcov --directory $(BIN_DIR) --capture --output-file $(BIN_DIR)/coverage.info --ignore-errors unsupported
	lcov --remove $(BIN_DIR)/coverage.info '$(TEST_DIR)/*' '/usr/*' --output-file $(BIN_DIR)/coverage.info --ignore-errors unused,unsupported
	lcov --list $(BIN_DIR)/coverage.info --ignore-errors unsupported
	@echo ""

# Clean build artifacts
.PHONY: clean
clean:
	rm -rf $(BIN_DIR)
