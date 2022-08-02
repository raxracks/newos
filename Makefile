AS = i686-elf-as
CC = i686-elf-gcc

C_SOURCES = $(wildcard src/*.c)
ASM_SOURCES = $(wildcard src/*.s)

NAME=newos

OBJ = ${C_SOURCES:.c=.o} ${ASM_SOURCES:.s=.o}

all: ${NAME}.iso

%.o: %.c
	${CC} -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Iinclude -Ofast

%.o: %.s
	${AS} $< -o $@

${NAME}.bin: ${OBJ}
	${CC} -T linker.ld -o $@ -ffreestanding -O2 -nostdlib $^ -lgcc

check-multiboot: ${NAME}.bin
	grub-file --is-x86-multiboot ${NAME}.bin

${NAME}.iso: check-multiboot
	rm -rf isodir/
	mkdir -p isodir/boot/grub
	cp ${NAME}.bin isodir/boot/${NAME}.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o ${NAME}.iso isodir

clean:
	rm -rf isodir/
	rm -rf src/*.o
	rm -rf *.iso
	rm -rf *.bin
