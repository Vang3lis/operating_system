#include "idt.h"

void set_idt_gate(int n, u32 handler)
{
	/* flag:
	 * +------+-+---+-----+
	 * |bits  |7|6 5| 4-0 |
	 * +------+-+---+-----+
	 * |denote|P|DPL|0110|
	 * +------+-+---+-----+
	 * 0x8e = 0b10001110
	 * */
	
	idt[n].low_offset = low_16(handler);
	idt[n].sel = KERNEL_CS;
	idt[n].always0 = 0;
	idt[n].flags = 0x8E;
	idt[n].high_offset = high_16(handler);
}

void set_idt()
{
	idt_reg.base = (u32) &idt;
	idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
	// Don't make the mistake of loading &idt -- always load &idt_reg
	__asm__ __volatile__ ("lidt (%0)": : "r" (&idt_reg));
}
