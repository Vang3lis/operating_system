#include "../drivers/screen.h"

void main() {
	clear_screen();
	
	int i = 0;
	for(i = 0; i < 24; i++)
	{
		char str[0x10] = {0};
		int_to_ascii(i, str);
		kprint_at(str, 0, i);
	}
	kprint_at("This text forces the kernel to scroll. Row 0 will disappear. ", 60, 24);
	kprint("And with this text, the kernel will scroll again, and row 1 will disappear too!");
}
