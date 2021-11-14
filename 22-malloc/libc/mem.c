#include "mem.h"

void memcpy(u8* source, u8* dest, int lenth)
{
	int i = 0;

	for(i = 0; i < lenth; i++)
		dest[i] = source[i];
}

void memset(u8* dest, char value, int lenth)
{
	int i = 0;

	for(i = 0; i < lenth; i++)
		dest[i] = value;
}

u32 free_mem_addr = 0x10000;

u32 kmalloc(u32 size, int align, u32* phys_addr)
{
	if(align == 1)
	{
		free_mem_addr = free_mem_addr&0xfff? (free_mem_addr&0xfffff000)+0x1000: free_mem_addr;
	}

	if(phys_addr)
		*phys_addr = free_mem_addr;
	
	u32 addr = free_mem_addr;

	free_mem_addr += size;
	return addr;
}
