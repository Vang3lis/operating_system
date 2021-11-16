#ifndef MEM_H
#define MEM_H

#include <stdint.h>

void memcpy(uint8_t* source, uint8_t* dest, int lenth);
void memset(uint8_t* dest, char value, int lenth);
uint32_t kmalloc(uint32_t size, int align, uint32_t* phys_addr);

#endif
