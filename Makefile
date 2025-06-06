BUILD_DIR := ./build
SRC_DIR := ./src
CC ?= clang
CFLAGS ?= -std=c99 -Wall -Werror -O2
CPPFLAGS ?= -I$(SRC_DIR)/include
TARGET := croskbd
SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SRCS:.c=.o))

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(BUILD_DIR) $(OBJS)
	@$(CC) -o $@ $(OBJS)

$(BUILD_DIR):
	@mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
