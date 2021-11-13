#include "kernel.h"
#include "../cpu/isr.h"
#include "../drivers/keyboard.h"
#include "../drivers/screen.h"
#include "../libc/string.h"

void main()
{
	clear_screen();
	isr_install();
	irq_install();

	kprint("Type something, it will be handled by kernel\n");
	kprint("Type END to halt the CPU\n\n");	
	kprint(">");
}

void user_input(char* input)
{
	if(strcmp(input, "END") == 0)
	{
		kprint("Stop the CPU.\n");
		asm volatile("hlt");
	}
	kprint("echo: ");
	kprint(input);
	kprint("\n>");
}

