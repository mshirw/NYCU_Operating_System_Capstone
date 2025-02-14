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
    memset((void*)ARM_DRAM_START, 0, 0xFFFFFFF);
    memset((void*)MBUF_BASE, 0, MBUF_SIZE);
    memset((void*)MEMINFO_BASE, 0, MEMINFO_SIZE);
    memset((void*)MEMFREELIST_BASE, 0, MEMFREELIST_SIZE);

    uint16 frame_list[16] = {1,1,1,1,2,2,4,4,8,8,16,16,32,32,64,64};//log2
    uint8 frame_index = 0;
    // Initialize frame_array
    frame_array *frame = (frame_array *)MEMINFO_BASE;
    free_frame_list *free_list = (free_frame_list *)MEMFREELIST_BASE;
    memset((void*)frame, 0, MEMINFO_SIZE);
    memset((void*)free_list, 0, MEMFREELIST_SIZE);

    free_frame_list *free_list_head = NULL;
    free_list_head = free_list;

    for(uint8 i = 0; i < sizeof(frame_list) / sizeof(frame_list[0]); i++)
    {
        /*uart_send_string("frame:");
        uart_binary_to_hex(calc_log2(frame_list[i]));
        uart_send_string("\r\n");*/

        //build free frame list
        for(uint8 k = 0; k < frame_list[i]; k++)
        {
            if(k == 0)
            {
                if(i == 0)
                    frame[frame_index].frame_start = MBUF_BASE;
                else
                    frame[frame_index].frame_start = frame[frame_index - 1].frame_start + frame[frame_index - 1].frame_size;
                
                frame[frame_index].frame_size = 4096;
                frame[frame_index].frame_num = calc_log2(frame_list[i]);
                frame[frame_index].is_free = 1;

                free_list_head = &free_list[frame[frame_index].frame_num];

                uart_send_string("frame_num:");
                uart_binary_to_hex(frame[frame_index].frame_num);
                uart_send_string("\r\n");

                if(free_list_head->frame == 0)
                {
                    uart_send_string("create list\r\n");
                    free_frame_list *next_frame = (free_frame_list *)malloc(sizeof(free_frame_list));
                    memset(next_frame, 0, sizeof(free_frame_list));

                    free_list[frame[frame_index].frame_num].frame = &frame[frame_index];
                    free_list[frame[frame_index].frame_num].next_free_frame = next_frame;
                }
                else
                {
                    uart_send_string("add node\r\n");
                    while(free_list_head->next_free_frame != 0)
                    {
                        free_list_head = free_list_head->next_free_frame;
                    }
                    free_frame_list *free_frame = (free_frame_list *)malloc(sizeof(free_frame_list));
                    memset(free_frame, 0, sizeof(free_frame_list));

                    free_list_head->frame = &frame[frame_index];
                    free_list_head->next_free_frame = free_frame;
                }
                
                /*uart_send_string("frame start: ");
                uart_binary_to_hex(frame[frame_index].frame_start);
                uart_send_string("\r\n");*/
                frame_index++;
            }
            else
            {
                frame[frame_index].frame_start = frame[frame_index - 1].frame_start + frame[frame_index - 1].frame_size;
                frame[frame_index].frame_size = 4096;
                frame[frame_index].frame_num = 0xFF;
                /*uart_send_string("contiguous frame\r\n");
                uart_send_string("frame start: ");
                uart_binary_to_hex(frame[frame_index].frame_start);
                uart_send_string("\r\n");*/
                frame_index++;
            }
        }
    }
}