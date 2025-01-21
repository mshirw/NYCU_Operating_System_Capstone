#ifndef _MEMALLOC_H
#define _MEMALLOC_H

#define ARM_DRAM_START 0x10000000
#define ARM_DRAM_END 0xBFFFFFFF

#define UART_RECEIVE_BUFFER_BASE 0x20000000
#define UART_RECEIVE_BUFFER_SIZE 1024
#define UART_TRANSMIT_BUFFER_BASE (UART_RECEIVE_BUFFER_BASE + UART_RECEIVE_BUFFER_SIZE)
#define UART_TRANSMIT_BUFFER_SIZE 1024

void *malloc(unsigned long size);

#endif  /*_MEMALLOC_H */