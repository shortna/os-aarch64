# in case shell inherited from environment
SHELL := /bin/bash

SRCDIR     := src
OBJDIR     := objects
TARGET_DIR := build

TARGET := k
INCLUDE_DIR = include

LD_SCRIPT := script.ld
CFLAGS    := -std=c99 -nostdlib -ffreestanding -Wall -Wextra -g -I $(INCLUDE_DIR)

CONSOLE_SOCKET :=/tmp/virt_console.socket
# uart connected to stdio, console connected to socket
QEMU_CONSOLE := -chardev socket,id=console0,path=$(CONSOLE_SOCKET),server=on,wait=on -device virtio-serial-device,max_ports=2 -device virtconsole,chardev=console0
# general configuration
QEMUFLAGS := -machine virt-9.0,gic-version=3 -cpu cortex-a57 -m 1G -no-reboot -nographic -serial mon:stdio $(QEMU_CONSOLE) 

CC := clang -target aarch64-none-elf
LD := ld.lld

SRCS    := $(wildcard $(SRCDIR)/*)
OBJECTS := $(patsubst $(SRCDIR)/%,$(OBJDIR)/%.o,$(SRCS))

.PHONY: all clean distclean qemu qemu-debug qemu-dtb tags
all: $(OBJDIR) $(TARGET_DIR) $(TARGET) 

$(OBJDIR):
	mkdir $@

$(TARGET_DIR):
	mkdir $@

$(OBJDIR)/%.o: $(SRCDIR)/%
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(LD) $^ -T $(LD_SCRIPT) -o $(TARGET_DIR)/$@

connect_to_console:
	socat UNIX-CONNECT:$(CONSOLE_SOCKET) STDOUT

clean:
	$(RM) -r $(OBJDIR) 

distclean:
	$(RM) -r $(OBJDIR) $(TARGET_DIR)

qemu: all
	qemu-system-aarch64 $(QEMUFLAGS) -kernel $(TARGET_DIR)/$(TARGET)

qemu-debug: all
	qemu-system-aarch64 -s -S $(QEMUFLAGS) -kernel $(TARGET_DIR)/$(TARGET)

qemu-dtb: 
	qemu-system-aarch64 $(QEMUFLAGS) -machine dumpdtb=dtb

