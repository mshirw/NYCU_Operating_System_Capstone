#include "timer.h"
#include "memalloc.h"
#include "utils.h"
#include "mini_uart.h"
#include "irq.h"
#include "str.h"

task_timer *timer_head = 0;

void send_message(char *message)
{
    uart_send_string(message);
}

void add_node(task_timer *timer)
{
    task_timer *temp_node = 0;

    if(timer_head == 0)
    {
        timer_head = timer;
        timer_head->prev = 0;
        timer_head->next = 0;
        // Reprogram the hardware timer
        asm volatile ("msr cntp_cval_el0, %0"::"r"(timer->expiry_time));
    }
    else
    {
        temp_node = timer_head;

        while(temp_node != 0)
        {
            if(temp_node->next == 0)
                break;

            if(timer->expiry_time < temp_node->expiry_time)
            {
                //insert before temp_node
                timer->next = temp_node;
                timer->prev = temp_node->prev;
                temp_node->prev = timer;
                // Reprogram the hardware timer
                asm volatile ("msr cntp_cval_el0, %0"::"r"(timer->expiry_time));
                return;
            }
            else
            {
                temp_node = temp_node->next;
            }
            
        }

        temp_node->next = timer;
        timer->prev = temp_node;
    }
}

void add_timer(timer_callback callback, uint32 sec, char *msg)
{
    task_timer *timer = (task_timer*)malloc(sizeof(task_timer));
    char *msg_data = utils_strdup(msg);

    uint64 current_time, cntfrq;

    timer->callback = callback;
    timer->data = msg_data;
    timer->next = 0;
    timer->prev = 0;
    
	asm volatile("mrs %0, cntpct_el0":"=r"(current_time));
	asm volatile("mrs %0, cntfrq_el0":"=r"(cntfrq));

    timer->expiry_time = current_time + sec * cntfrq;
    //asm volatile ("msr cntp_tval_el0, %0"::"r"(sec * cntfrq));//set timer
    
    add_node(timer);

    //enable timer
    put32(CORE0_TIMER_IRQ_CTRL, 2);
}