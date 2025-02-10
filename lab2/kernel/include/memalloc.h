#ifndef _MEMALLOC_H
#define _MEMALLOC_H
#include "utils.h"

//buddy system memory allocation

#define ARM_DRAM_START 0x10000000
#define ARM_DRAM_END 0xBFFFFFFF

#define UART_RECEIVE_BUFFER_BASE 0x20000000
#define UART_RECEIVE_BUFFER_SIZE 1024
#define UART_TRANSMIT_BUFFER_BASE (UART_RECEIVE_BUFFER_BASE + UART_RECEIVE_BUFFER_SIZE)
#define UART_TRANSMIT_BUFFER_SIZE 1024
#define MEMINFO_BASE (UART_TRANSMIT_BUFFER_BASE + UART_TRANSMIT_BUFFER_SIZE)
#define MEMINFO_SIZE 0x1000
#define MBUF_BASE (MEMINFO_BASE + MEMINFO_SIZE)
#define MBUF_SIZE 0x10000//64KB

typedef struct frame_array
{
    uint32 frame_start;
    uint16 frame_size;//in bytes
    uint16 frame_num;
    uint8 frame_tag;
    uint8 is_free;

} frame_array;

void *malloc(unsigned long size);
void memalloc_init();
uint8 calc_log2(uint8 value);

#endif  /*_MEMALLOC_H */