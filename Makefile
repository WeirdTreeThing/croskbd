BUILD_DIR := ./build
SRC_DIR := ./src
CC ?= cc
CFLAGS ?= -std=c2x -O2 -Wall -Werror -pedantic -Wno-missing-braces -Wno-unused-result -Wno-overflow
CPPFLAGS ?= -I$(SRC_DIR)/include
PREFIX ?= /usr/local
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

.PHONY: clean install install_dinit install_systemd

install:
	@install -Dm 755 $(BUILD_DIR)/$(TARGET) $(PREFIX)/bin/$(TARGET)

install_dinit:
	@sed 's|PREFIX|$(PREFIX)|' ./data/$(TARGET).dinit.in > $(BUILD_DIR)/$(TARGET).dinit
	@install -Dm644 $(BUILD_DIR)/$(TARGET).dinit $(PREFIX)/lib/dinit.d/$(TARGET)

install_systemd:
	@sed 's|PREFIX|$(PREFIX)|' ./data/$(TARGET).systemd.in > $(BUILD_DIR)/$(TARGET).systemd
	@install -Dm644 $(BUILD_DIR)/$(TARGET).systemd $(PREFIX)/lib/systemd/system/$(TARGET).service

clean:
	rm -rf $(BUILD_DIR)
