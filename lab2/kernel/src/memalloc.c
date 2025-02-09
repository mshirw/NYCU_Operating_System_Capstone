#include "memalloc.h"

void *malloc(unsigned long size)
{
    static unsigned long int memory = ARM_DRAM_START;
    unsigned long int mem;

    if(memory >= ARM_DRAM_END)
    {
        memory = ARM_DRAM_START;
    }

    mem = memory;
    memory += size;

    return (void *)mem;
}

void memalloc_init()
{
    // Initialize memory
    unsigned long int *mem = (unsigned long int *)MBUF_BASE;
    for(int i = 0; i < MBUF_SIZE / sizeof(unsigned long int); i++)
    {
        mem[i] = 0;
    }

    uint16 frame_list[16] = {1,1,1,1,2,2,4,4,8,8,16,16,32,32,64,64};
    // Initialize frame_array
    frame_array *frame = (frame_array *)MEMINFO_BASE;

    for(int i = 0; i < 16; i++)
    {
        
    }
}