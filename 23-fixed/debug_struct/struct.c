// gcc -m32 -g struct.c -o struct

#include <stdint.h>

struct registers_t
{
	uint32_t ds; // Data segment selector
	uint32_t edi, esi, ebp, unless, ebx, edx, ecx, eax; // Pushed by pusha
	uint32_t int_no, err_code; // Interrupt number and error code (if applicable)
	uint32_t eip, cs, eflags, esp, ss; // Pushed by the processor automatically
};

int main()
{
	struct registers_t r;
	r.ds = 1;
	return 0;
}
