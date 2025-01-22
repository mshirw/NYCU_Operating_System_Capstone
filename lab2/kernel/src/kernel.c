#include "mini_uart.h"
#include "mailbox.h"
#include "cpio_parser.h"
#include "memalloc.h"
#include "irq.h"

extern char __kernel_start[];
extern char __kernel_end[];

void kernel_main(void)
{
	//volatile const unsigned char *cpio_address = 0x20000000; //raspberry pi 3b+ use this address
	volatile const unsigned char *cpio_address = 0x8000000; //QEMU sim use this address

	//uart_binary_to_hex(__kernel_start);
	//uart_binary_to_hex(__kernel_end);

	put32(CORE0_TIMER_IRQ_CTRL, 0);//disable timer

	uart_init();
	uart_send_string("Hello, world!\r\n");
	uart_send_string("# ");
	parse_cpio(cpio_address);
	//must wait all initialization done before enable uart interrupt
	uart_enable_interrupt();

	while(1);
}
