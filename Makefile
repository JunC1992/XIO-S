#!Makefile
C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_SOURCES = $(shell find . -name "*.s")
S_OBJECTS = $(patsubst %.s, %.o, $(S_SOURCES))

CC = gcc
LD = ld
ASM = nasm
TARGET = xiaos

C_FLAGS = -c -Wall -m32 -ggdb -gstabs+ -nostdinc -fno-builtin -fno-stack-protector -I include
LD_FLAGS = -T link.ld -m elf_i386 -nostdlib
ASM_FLAGS = -f elf -g -F stabs

all: $(S_OBJECTS) $(C_OBJECTS) xiao burn_image

# compile
.c.o:
	$(CC) $(C_FLAGS) $< -o $@

.s.o:
	$(ASM) $(ASM_FLAGS) $<

.PHONY:xiao
xiao:
	$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o $(TARGET)

.PHONY:clean
clean:
	$(RM) $(S_OBJECTS) $(C_OBJECTS) $(TARGET)

.PHONY:burn_image
burn_image:
	mkdir -p /mnt/kernel
	mount floppy.img /mnt/kernel
	cp $(TARGET) /mnt/kernel/$(TARGET)
	sleep 1
	umount /mnt/kernel

run:
	#qemu -fda floppy.img -boot a -nographic
	qemu -fda floppy.img -boot a -curses 
	#add '-nographic' option if using server of linux distro, such as fedora-server,or "gtk initialization failed" error will occur.

debug:
	qemu -S -s -fda floppy.img -boot a -curses 

