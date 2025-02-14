#include "utils.h"
#include "peripherals/mini_uart.h"
#include "peripherals/gpio.h"
#include "reboot.h"
#include "mailbox.h"
#include "cpio_parser.h"
#include "irq.h"
#include "timer.h"
#include "mini_uart.h"
#include "sprintf.h"

char **cmdArr[CMD_NUM];
// get address from linker
extern volatile unsigned char _end;

void uart_send ( char c )
{
	while(1) {
		if(get32(AUX_MU_LSR_REG)&0x20) 
			break;
	}
	put32(AUX_MU_IO_REG,c);
}

char uart_recv ( void )
{
	while(1) {
		if(get32(AUX_MU_LSR_REG)&0x01) 
			break;
	}
	return(get32(AUX_MU_IO_REG)&0xFF);
}

void uart_send_string(char* str)
{
	for (int i = 0; str[i] != '\0'; i ++) {
		uart_send((char)str[i]);
	}
}

void uart_send_string_length(char* str, unsigned int length)
{
	for (int i = 0; i < length; i ++) {
		uart_send((char)str[i]);
	}
}

void uart_cmd_help()
{
	uart_send_string("help: print this help menu\r\n");
	uart_send_string("hello: print Hello World!\r\n");
	uart_send_string("reboot: reboot the device\r\n");
	uart_send_string("board revision: get board revision\r\n");
	uart_send_string("ARM memory: get ARM memory address\r\n");
}

void uart_cmd_hello()
{
	uart_send_string("Hello World!\r\n");
}

void uart_cmd_reboot()
{
	uart_send_string("reboot...\r\n");
	reset(100);
}

void uart_cmd_parser(int cmdNum)
{
	switch (cmdNum)
	{
	case 0:
		uart_cmd_help();
		break;
	case 1:
		uart_cmd_hello();
		break;
	case 2:
		uart_cmd_reboot();
		break;
	case 3:
		get_board_revision();
		break;
	case 4:
		get_ARM_memory();
		break;
	case 5:
		uart_send_string("List all files names\r\n");
		list_file_names();
		//parse_cpio(cpio_address, cmdNum, "");
		break;
	case 6:
		uart_send_string("Read file: ");
		//files handle were implemented in uart interrupt handler.
		break;
	case 7:
		add_timer(send_message, 3, "timer 3 timerout");
		add_timer(send_message, 2, "timer 2 timerout");
		add_timer(send_message, 1, "timer 1 timerout");
		//enable timer
    	put32(CORE0_TIMER_IRQ_CTRL, 2);
    	asm volatile("msr cntp_ctl_el0, %0"::"r"(0x1));
		break;
	
	default:
		break;
	}
}

unsigned int uart_get_string_length(char *str1)
{
	int len = 0;

	while(str1[len] != '\0')
	{
		len++;
	}

	return len;
}

unsigned int uart_cmp_string(char *uart_str, char *cmd_str)
{
	int i = 0, result = 1;

	if(uart_get_string_length(uart_str) < uart_get_string_length(cmd_str))
	{
		return 0;
	}

	while((uart_str[i] != '\0') && (cmd_str[i] != '\0'))
	{
		if(uart_str[i] != cmd_str[i])
		{
			result = 0;
			break;
		}
		else
		{
			i++;
			result = 1;
		}
	}

	return result;
}

unsigned int uart_cmp_string_length(char *uart_str, char *cmd_str, unsigned int length)
{
	int i = 0, result = 1;

	while(i < length)
	{
		if(uart_str[i] != cmd_str[i])
		{
			result = 0;
			break;
		}
		else
		{
			i++;
			result = 1;
		}
	}

	return result;
}

void uart_binary_to_hex(unsigned int d)
{
    unsigned int n;
    int c;
    uart_send_string("0x");
    for(c=28;c>=0;c-=4) {
        // get highest tetrad
        n=(d>>c)&0xF;
        // 0-9 => '0'-'9', 10-15 => 'A'-'F'
        n+=n>9?0x37:0x30;
        uart_send(n);
    }
	uart_send_string("\r\n");
}

void uart_enable_interrupt() {
    
    // Enable RX and TX interrupt for mini UART
    uint32 ier = get32(AUX_MU_IER_REG);
    //enable Receive interrupt
    ier |= RX_INTERRUPT_BIT; 
    // enable both receive and transmit interrupt
    //ier |= (RX_INTERRUPT_BIT | TX_INTERRUPT_BIT); 

    put32(AUX_MU_IER_REG, ier);
	
    // Enable the mini UART interrupt in the second-level interrupt controller
    uint32 enable_irqs1 = (uint32) ENABLE_IRQS_1;// ENABLE_IRQS_1 defined in irq.h
    enable_irqs1 |= AUXINIT_BIT_POSTION; // Set bit29
    put32(ENABLE_IRQS_1, enable_irqs1);
}

void uart_init_cmd()
{
	static char cmdTextArr[CMD_NUM][32] = {
		"help",
		"hello",
		"reboot",
		"board revision",
		"ARM memory",
		"ls",
		"cat",
		"timer"
	};

	for(uint8 index = 0; index < CMD_NUM; index++)
	{
		cmdArr[index] = &cmdTextArr[index];
	}
}

/**
 * Display a string
 */
void printf(char *fmt, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, fmt);
    // we don't have memory allocation yet, so we
    // simply place our string after our code
    char *s = (char*)&_end;
    // use sprintf to format our string
    vsprintf(s,fmt,args);
    // print out as usual
    while(*s) {
        /* convert newline to carrige return + newline */
        if(*s=='\n')
            uart_send('\r');
        uart_send(*s++);
    }
}

void uart_init ( void )
{
	unsigned int selector;

	selector = get32(GPFSEL1);
	selector &= ~(7<<12);                   // clean gpio14
	selector |= 2<<12;                      // set alt5 for gpio14
	selector &= ~(7<<15);                   // clean gpio15
	selector |= 2<<15;                      // set alt5 for gpio15
	put32(GPFSEL1,selector);

	put32(GPPUD,0);
	delay(150);
	put32(GPPUDCLK0,(1<<14)|(1<<15));
	delay(150);
	put32(GPPUDCLK0,0);

	put32(AUX_ENABLES,1);                   //Enable mini uart (this also enables access to its registers)
	put32(AUX_MU_CNTL_REG,0);               //Disable auto flow control and disable receiver and transmitter (for now)
	put32(AUX_MU_IER_REG,0);                //Disable receive and transmit interrupts
	put32(AUX_MU_LCR_REG,3);                //Enable 8 bit mode
	put32(AUX_MU_MCR_REG,0);                //Set RTS line to be always high
	put32(AUX_MU_BAUD_REG,270);             //Set baud rate to 115200

	put32(AUX_MU_CNTL_REG,3);               //Finally, enable transmitter and receiver

	uart_init_cmd();
}
