# in case shell inherited from environment
SHELL := /bin/bash

SRCDIR     := src
OBJDIR     := objects
TARGET_DIR := build

TARGET := k
INCLUDE_DIR = include

LD_SCRIPT := script.ld
CFLAGS    := -nostdlib -ffreestanding -Wall -Wextra -g -I $(INCLUDE_DIR)
QEMUFLAGS := -nographic -no-reboot -machine virt -cpu cortex-a57 -m 32

CC      := ../aarch64-none-elf/bin/aarch64-none-elf-gcc
AS      := ../aarch64-none-elf/bin/aarch64-none-elf-as
LD      := ../aarch64-none-elf/bin/aarch64-none-elf-ld
OBJCOPY := ../aarch64-none-elf/bin/aarch64-none-elf-objcopy

SRCS    := $(wildcard $(SRCDIR)/*)
OBJECTS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%.o,$(SRCS))

.PHONY: all clean distclean qemu qemu-debug
all:$(TARGET_DIR) $(TARGET) 

$(OBJDIR):
	mkdir $@

$(TARGET_DIR):
	mkdir $@

$(OBJDIR)/%.o: $(SRCDIR)/% $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(LD) $^ -T $(LD_SCRIPT) -o $(TARGET_DIR)/$@

clean:
	$(RM) -r $(OBJDIR) 

distclean:
	$(RM) -r $(OBJDIR) $(TARGET_DIR)

qemu: $(TARGET_DIR) $(TARGET)
	qemu-system-aarch64 $(QEMUFLAGS) -kernel $(TARGET_DIR)/$(TARGET)

qemu-debug: $(TARGET_DIR) $(TARGET)
	qemu-system-aarch64 -s -S $(QEMUFLAGS) -kernel $(TARGET_DIR)/$(TARGET)
