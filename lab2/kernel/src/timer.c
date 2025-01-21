#include "timer.h"
#include "memalloc.h"
#include "utils.h"
#include "mini_uart.h"
#include "irq.h"

task_timer *timer_head;

void send_message(char *message)
{
    uart_send_string(message);
}

void create_timer()
{
    add_timer(send_message, 3);
}

void add_timer(timer_callback callback, uint32 timeout)
{
    task_timer *timer = (task_timer*)malloc(sizeof(task_timer));
    uint64 current_time, cntfrq;

    timer->callback = callback;
    uart_send_string("callback addr:\r\n");
    uart_binary_to_hex(timer->callback);
    uart_send_string("\r\n");
    
	asm volatile("mrs %0, cntpct_el0":"=r"(current_time));
	asm volatile("mrs %0, cntfrq_el0":"=r"(cntfrq));

    timer->expiry_time = current_time + timeout * cntfrq;
    asm volatile ("msr cntp_tval_el0, %0"::"r"(timeout * cntfrq));//set timer
    
    timer_head = timer;
    //enable timer
    put32(CORE0_TIMER_IRQ_CTRL, 2);
}