#include "mini_uart.h"

extern char __bootloader_start[];
extern char __bootloader_end[];

void bootloader_main()
{
    char value;
    unsigned int data_size = 0;
    volatile unsigned char *kernel_address = 0x80000;
    uart_init();

    uart_send_string("bootloader...\r\n");
    uart_binary_to_hex(__bootloader_start);
    uart_binary_to_hex(__bootloader_end);
    //flush out uart receive buffer
    for(unsigned char i = 0; i < 8; i++)
        value = uart_recv();

    while(1)
    {
        uart_send_string("wait to get data size...\r\n");
        for(unsigned char i = 0; i < 4; i++)
        {
            value = uart_recv();
            data_size |= value;
            if(i < 3)
                data_size = data_size << 8;
        }
        uart_send_string("data size: ");
        uart_binary_to_hex(data_size);
        uart_send_string("Start to load the kernel image... \r\n");
        
        while(data_size--)
        {
            *kernel_address++ = uart_recv();
        }
        uart_send_string("transfer done!\r\n");
        uart_send_string("jump to kernel...\r\n");
        break;
    }

    asm volatile
    (
        "mov x30, 0x80000;"
        "ret;"
    );
}