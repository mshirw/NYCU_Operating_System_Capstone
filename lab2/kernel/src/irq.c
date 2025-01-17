# include "irq.h"
# include "mini_uart.h"

void irq_except_handler_c()
{
    uart_send_string("In timer interruption\n");

	unsigned long long cntpct_el0 = 0;//The register count secs with frequency
	asm volatile("mrs %0,cntpct_el0":"=r"(cntpct_el0));
	
	//unsigned long long cntfrq_el0 = 400;//The base frequency
    unsigned long long cntfrq_el0 = 0;//The base frequency
	asm volatile("mrs %0,cntfrq_el0":"=r"(cntfrq_el0));

	unsigned long long sec = cntpct_el0 / cntfrq_el0;
	uart_send_string("sec:");
	uart_binary_to_hex(sec);
	uart_send_string("\n");
	
	//unsigned long long wait = 800;// wait 2 seconds
    unsigned long long wait = cntfrq_el0 * 2;// wait 2 seconds
	asm volatile ("msr cntp_tval_el0, %0"::"r"(wait));//set new timer
}

void except_handler_c()
{
    uart_send_string("Exception received!\r\n");
}