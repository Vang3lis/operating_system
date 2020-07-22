# $@ = target file
# $< = first dependency
# $^ = all denpendencies

# First rule is the one excuted when no paramters are fed to the Makefile
all: run

kernel.bin: kernel_entry.o kernel.o
	# ld -m elf_i386 -o kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o --oformat binary
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary
	

kernel_entry.o: kernel_entry.asm
	nasm $< -f elf -o $@

kernel.o: kernel.c
	gcc -m32 -ffreestanding -fno-pie -c $< -o $@

bootsect.bin: bootsect.asm
	nasm $< -f bin -o $@

os-image: bootsect.bin kernel.bin
	cat $^ > $@

run: os-image
	qemu-system-i386 -fda $<

debug: os-image
	qemu-system-i386 -s -S -fda $<

clean: 
	rm *.bin *.o
