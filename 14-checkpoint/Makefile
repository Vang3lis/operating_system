C_SOURCES = $(wildcard kernel/*.c)

OBJ = $(C_SOURCES:.c=.o)

CC = gcc
GDB = gdb 

CFLAGS = -m32 -g -fno-pie

os-image: boot/bootsect.bin kernel.bin 
	cat $^ > os-image

# '--oformat binary' deletes all symbols as a collateral 
kernel.bin: boot/kernel_entry.o $(OBJ)
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

# use for debug 
kernel.elf: boot/kernel_entry.o $(OBJ)
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ 

run: os-image
	qemu-system-i386 -fda os-image 

debug: os-image
	qemu-system-i386 -s -S -fda os-image & peda & gdb -ex "set architecture i8086" -ex "target remote:1234" -ex "b *0x7c00" -ex "c"

%.o: %.c
	$(CC) $(CFLAGS) -ffreestanding -c $<  -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean: 
	rm -rf *.bin *.dis *.o *.elf
	rm -rf ./kernel/*.o ./boot/*.o ./boot/*.bin

full-clean:
	rm -rf *.bin *.dis *.o *.elf os-image
	rm -rf ./kernel/*.o ./boot/*.o ./boot/*.bin
	rm -rf ./peda* ./.gdb_history

