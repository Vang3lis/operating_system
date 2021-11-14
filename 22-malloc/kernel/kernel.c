#include "kernel.h"
#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../libc/mem.h"

void main()
{
	clear_screen();
	isr_install();
	irq_install();

	kprint("Type something, it will be handled by kernel\n");
	kprint("Type END to halt the CPU\n");	
	kprint("Type PAGE to get a freed kernel address\n\n");
	kprint(">");
}

void user_input(char* input)
{
	if(strcmp(input, "END") == 0)
	{
		kprint("Stop the CPU.\n");
		asm volatile("hlt");
	}
	else if(strcmp(input, "PAGE") == 0)
	{
		u32 phys_addr = 0;
		u32 page = kmalloc(0x800, 1, &phys_addr);
		char str[0x10] = {0};

		hex_to_ascii(page, str);
		kprint("Page: ");
		kprint(str);
		kprint("\n");

		hex_to_ascii(phys_addr, str);
		kprint("Physical addr: ");
		kprint(str);
		kprint("\n");
	}
	kprint("echo: ");
	kprint(input);
	kprint("\n>");
}

