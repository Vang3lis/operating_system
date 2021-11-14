#ifndef MEM_H
#define MEM_H

#include "../cpu/types.h"

void memcpy(u8* source, u8* dest, int lenth);
void memset(u8* dest, char value, int lenth);
u32 kmalloc(u32 size, int align, u32* phys_addr);

#endif
