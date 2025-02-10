#include "memalloc.h"
#include "str.h"
#include "mini_uart.h"

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

uint8 calc_log2(uint8 value)
{
    uint8 pow = 0;
    while(value != 0)
    {
        value >>= 1;
        pow++;
    }
    return pow - 1;
}

void memalloc_init()
{
    // Initialize memory
    memset((void*)MBUF_BASE, 0, MBUF_SIZE);
    memset((void*)MEMINFO_BASE, 0, MEMINFO_SIZE);

    uint16 frame_list[16] = {1,1,1,1,2,2,4,4,8,8,16,16,32,32,64,64};//log2
    uint8 frame_index = 0;
    // Initialize frame_array
    frame_array *frame = (frame_array *)MEMINFO_BASE;

    for(uint8 i = 0; i < 16; i++)
    {
        uart_send_string("frame:");
        uart_binary_to_hex(calc_log2(frame_list[i]));
        uart_send_string("\r\n");

        for(uint8 k = 0; k < frame_list[i]; k++)
        {
            if(k == 0)
            {
                if(i == 0)
                    frame[frame_index].frame_start = MBUF_BASE;
                else
                    frame[frame_index].frame_start = frame[frame_index - 1].frame_start + frame[frame_index - 1].frame_size;
                
                frame[frame_index].frame_size = 4096;
                frame[frame_index].frame_num = calc_log2(frame_list[frame_index]);
                uart_send_string("frame start: ");
                uart_binary_to_hex(frame[frame_index].frame_start);
                uart_send_string("\r\n");
                frame_index++;
            }
            else
            {
                frame[frame_index].frame_start = frame[frame_index - 1].frame_start + frame[frame_index - 1].frame_size;
                frame[frame_index].frame_size = 4096;
                frame[frame_index].frame_num = ~(calc_log2(frame_list[frame_index]));
                uart_send_string("contiguous frame\r\n");
                uart_send_string("frame start: ");
                uart_binary_to_hex(frame[frame_index].frame_start);
                uart_send_string("\r\n");
                frame_index++;
                /*uart_send_string("contiguous frame num");
                uart_binary_to_hex(frame[i].frame_num);
                uart_send_string("\r\n");*/
            }
        }
    }
}