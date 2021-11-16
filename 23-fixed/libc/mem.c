#include "mem.h"

void memcpy(uint8_t* source, uint8_t* dest, int lenth)
{
	int i = 0;

	for(i = 0; i < lenth; i++)
		dest[i] = source[i];
}

void memset(uint8_t* dest, char value, int lenth)
{
	int i = 0;

	for(i = 0; i < lenth; i++)
		dest[i] = value;
}

uint32_t free_mem_addr = 0x10000;

uint32_t kmalloc(uint32_t size, int align, uint32_t* phys_addr)
{
	if(align == 1)
	{
		free_mem_addr = free_mem_addr&0xfff? (free_mem_addr&0xfffff000)+0x1000: free_mem_addr;
	}

	if(phys_addr)
		*phys_addr = free_mem_addr;
	
	uint32_t addr = free_mem_addr;

	free_mem_addr += size;
	return addr;
}
