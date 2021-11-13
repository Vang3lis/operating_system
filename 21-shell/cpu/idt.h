#ifndef IDT_H
#define IDT_H

#include "types.h"

// Segment selectors
#define KERNEL_CS 0x08

/* How every interrupt gate (handler) is defined
 * We use the attribute 'packed' to tell GCC not to change
 * any of the alignment in the structure
 */
typedef struct {
	u16 low_offset; // lower 16bits of handler function address
	u16 sel; // kernel segment selector
	u8  always0;
	/* flag:
	 * +------+-+---+-----+
	 * |bits  |7|6 5| 4-0 |
	 * +------+-+---+-----+
	 * |denote|P|DPL|01110|
	 * +------+-+---+-----+
	 * 
	 * Bit 7: "Interrupt is present"
	 * Bits 6-5: Privilege level of caller (0=ring0 kernel 3=ring3 user)
	 * Bit 4: Set to 0 for interrupt gates
	 * Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate"
	 */
	u8 flags;
	u16 high_offset; // higher/upper 16 bits of handler function address
} __attribute__((packed)) idt_gate_t;

/* A pointer to the array of interrupt handlers.
 * Assembly instrcution 'lidt' will read it -- lidt [eax]
 * it just like 'lgdt [gdt_desciptor]'
 * */
typedef struct {
	u16 limit; // (The size of idt[] array) - 1
	u32 base; // The address of the first element in out idt[] array.
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256
idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

// Function implemented in idt.c
void set_idt_gate(int n, u32 handler);
void set_idt();

#endif
