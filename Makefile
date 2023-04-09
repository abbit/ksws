CC = g++
CFLAGS = -Wall -Wextra -Werror -Wpedantic -std=c++17
BUILD_DIR = build
TARGET = ksws
SRC = $(shell find . -type f -name "*.cpp")

.PHONY: all build run clean

all: clean build run

build: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

run:
	./$(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

