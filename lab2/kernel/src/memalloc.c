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