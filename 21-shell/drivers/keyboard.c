#include "keyboard.h"
#include "screen.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "../libc/function.h"
#include "../kernel/kernel.h"

#define SC_MAX 0x39
#define BACKSPACE 0xe
#define ENTER 0x1c

char input_buffer[0x100];

const char MAP[0x50] = "  1234567890-+  QWERTYUIOP[]  ASDFGHJKL;'` \\ZXCVBNM,./    ";

static void keyboard_callback(registers_t regs)
{
    char sc_ascii[4] = {0};

    u8 scancode = port_byte_in(0x60);
    int_to_ascii((int)scancode, sc_ascii);

	if(scancode > SC_MAX)
		return;
	if(scancode == BACKSPACE)
	{
		if(strlen(input_buffer))
		{
			backspace(input_buffer);
			kprint_backspace();
		}
	}
	else if(scancode == ENTER) {
		kprint("\n");
		user_input(input_buffer);
		memset(input_buffer, 0, 0x100);
	}
	else {
		char letter = MAP[scancode];
		char str[2] = {letter, '\0'};
		add(input_buffer, letter);
		kprint(str);
	}

	UNUSED(regs);
}

void init_keyboard()
{
    register_interrupt_handler(IRQ1, keyboard_callback);
}

