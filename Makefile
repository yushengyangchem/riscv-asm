BUILD_DIR := build
TARGET := $(BUILD_DIR)/riscv-asm-lab

CROSS_COMPILE ?= riscv64-unknown-linux-gnu-
CC := $(CROSS_COMPILE)gcc
OBJDUMP := $(CROSS_COMPILE)objdump
QEMU ?= qemu-riscv64

CFLAGS := -std=c11 -Wall -Wextra -O2 -g
LDFLAGS :=

SRCS := src/main.c src/asm_funcs.S
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

.PHONY: all run dump clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.S.o: %.S
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(TARGET)
	$(QEMU) ./$(TARGET)

dump: $(TARGET)
	$(OBJDUMP) -drwC ./$(TARGET) > $(BUILD_DIR)/riscv-asm-lab.dump
	sed -n '1,220p' $(BUILD_DIR)/riscv-asm-lab.dump

clean:
	rm -rf $(BUILD_DIR)
