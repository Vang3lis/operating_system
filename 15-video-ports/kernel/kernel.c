#include "../drivers/ports.h"

void main() {
	
	port_byte_out(0x3d4, 14);
	
	int position = port_byte_in(0x3d5);
	position = position << 8;

	port_byte_out(0x3d4, 15);
	position += port_byte_in(0x3d5);

	int offset_from_vga = position * 2;

	char *vga = 0xb8000;
	vga[offset_from_vga] = 'X';
	vga[offset_from_vga+1] = 0x0f;
	
	port_byte_out(0x3d4, 14);
	port_byte_out(0x3d5, 0);
	port_byte_out(0x3d4, 15);
	port_byte_out(0x3d5, 0);
	
	int offset = 0;
	offset = port_byte_in(0x3d5);
	offset = offset << 8;
	port_byte_out(0x3d4, 15);
	offset += port_byte_in(0x3d5);
	offset *= 2;

	vga[offset] = 'Y';
	vga[offset+1] = 0x0f;
	
}
