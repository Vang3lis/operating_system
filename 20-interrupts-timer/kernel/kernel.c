#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"
#include "../drivers/screen.h"

void main()
{
	clear_screen();
	isr_install();
	
	// enable interrupts again
	__asm__ __volatile__ ("sti");
	init_timer(50);
	init_keyboard();
}