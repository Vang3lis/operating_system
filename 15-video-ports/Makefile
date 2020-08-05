C_SOURCES = $(wildcard kernel/*.c drivers/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h)

OBJ = $(C_SOURCES:.c=.o)

CC = gcc
GDB = gdb

CFLAGS = -m32 -g -fno-pie

os-image: boot/bootsect.bin kernel.bin
	cat $^ > os-image

kernel.bin: boot/kernel_entry.o $(OBJ)
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: boot/kernel_entry.o $(OBJ)
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^

run: os-image
	qemu-system-i386 -fda os-image

debug: os-image
	qemu-system-i386 -s -S -fda os-image& \
	# gdb -ex "set architecture i8086" -ex "target remote:1234" -ex "symbol-file kernel.elf" -ex "b *0x7c00" -ex "c"
	gdb -x ./script

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean: 
	rm -rf *.bin *.o *.elf
	rm -rf ./kernel/*.o drivers/*.o ./boot/*.o

full-clean:
	rm -rf *.bin *.o *.elf
	rm -rf ./kernel/*.o ./drivers/*.o ./boot/*.o
	rm -rf ./peda* ./.gdb_history
	rm -rf os-image
