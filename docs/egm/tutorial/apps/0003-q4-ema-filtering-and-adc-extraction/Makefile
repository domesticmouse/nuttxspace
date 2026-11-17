CC ?= cc
CFLAGS := -std=c99 -Wall -Wextra -Werror -pedantic -O2
BUILD_DIR := build
BIN := $(BUILD_DIR)/bargraph_lesson

.PHONY: all test clean

all: $(BIN)

$(BIN): src/main.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) src/main.c -o $(BIN)

test: $(BIN)
	./$(BIN)

clean:
	rm -rf $(BUILD_DIR)
