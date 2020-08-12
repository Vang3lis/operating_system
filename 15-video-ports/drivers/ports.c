// Read a byte from the specified port 

/* inline assembler symtax
 * asm ( assembler template
 * : output operands
 * : input operands
 * : list of clobbered registers
 * )
 */ 

unsigned char port_byte_in (unsigned short port) {
	unsigned char result;
	/* in: input from port */

	// intel 
	// __asm__("in al, dx": "=a" (result): "d" (port));
	
	// at&t
	__asm__("in %%dx, %%al": "=a" (result): "d" (port));
	
	return result; 
}

void port_byte_out (unsigned short port, unsigned char data) {
	/* out: ouput to port */
	
	// intel
	// __asm__("out dx, al": : "a" (data), "d" (port));
	
	// at&t
	__asm__("out %%al, %%dx": : "a" (data), "d" (port));
}

unsigned short port_word_in (unsigned short port) {
	unsigned short result;
	
	// __asm__("in ax, dx": "=a" (result): "d" (port));
	
	__asm__("in %%dx, %%ax": "=a" (result): "d" (port));
	
	return result;
}

void port_word_out (unsigned short port, unsigned short data) {
	//__asm__("out dx, ax": : "a" (data), "d" (port));
	
	__asm__("out %%ax, %%dx": : "a" (data), "d" (port));
}
