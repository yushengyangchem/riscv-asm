BUILD_DIR := build
TARGET := $(BUILD_DIR)/riscv-asm-lab
TEST_TARGET := $(BUILD_DIR)/riscv-asm-tests

CROSS_COMPILE ?= riscv64-unknown-linux-gnu-
CC := $(CROSS_COMPILE)gcc
OBJDUMP := $(CROSS_COMPILE)objdump
QEMU ?= qemu-riscv64

CFLAGS := -std=c11 -Wall -Wextra -O2 -g
LDFLAGS :=

ASM_SRCS := src/asm_funcs.S \
	src/asm_arith.S \
	src/asm_branch.S \
	src/asm_memory.S \
	src/asm_stack.S

SRCS := src/main.c $(ASM_SRCS)
TEST_SRCS := src/test_asm.c $(ASM_SRCS)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
TEST_OBJS := $(TEST_SRCS:%=$(BUILD_DIR)/%.o)

.PHONY: all run test dump clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.S.o: %.S
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

run: $(TARGET)
	$(QEMU) ./$(TARGET)

test: $(TEST_TARGET)
	$(QEMU) ./$(TEST_TARGET)

dump: $(TARGET)
	$(OBJDUMP) -drwC ./$(TARGET) > $(BUILD_DIR)/riscv-asm-lab.dump
	sed -n '1,220p' $(BUILD_DIR)/riscv-asm-lab.dump

clean:
	rm -rf $(BUILD_DIR)
