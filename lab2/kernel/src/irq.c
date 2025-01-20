# include "irq.h"
# include "peripherals/mini_uart.h"
# include "mini_uart.h"
# include "utils.h"
# include "memalloc.h"


void uart_exception_handler_c()
{
	static uint16 dataHead = 0;
	static uint8 catFlag = 0;
	uint8 *dataBuffer = (uint8 *)UART_RECEIVE_BUFFER_BASE;
	char *cmdHelp = "# help";
	char *cmdHello = "# hello";
	char *cmdReboot = "# reboot";
	char *cmdGetBoardRevision = "# board revision";
	char *cmdGetARMMemory = "# ARM memory";
	char *cmdListFileNames = "# ls";
	char *cmdReadFile = "# cat";
	char *cmdPtr;
	char **cmdArr[7] = {cmdHelp, cmdHello, cmdReboot, cmdGetBoardRevision, cmdGetARMMemory, cmdListFileNames, cmdReadFile};
	int supportCmdNum = 7;

	unsigned int iir = get32(AUX_MU_IIR_REG);
	/*uart_send_string("In uart interruption\n");
	uart_send_string("iir:");
	uart_binary_to_hex(iir);
	uart_send_string("\r\n");*/

	dataBuffer[dataHead++] = uart_recv();
	uart_send(dataBuffer[dataHead-1]);

	if(dataBuffer[dataHead-1] == '\r')
	{
		dataHead = 0;
		uart_send_string("\n");

		if(catFlag)
		{
			catFlag = 0;
			print_file_data(dataBuffer);
		}

		for(int cmd = 0; cmd < supportCmdNum; cmd++)
		{
			cmdPtr = cmdArr[cmd];
			if(uart_cmp_string(dataBuffer, cmdPtr))
			{
				if(cmd == 6)
					catFlag = 1;

				uart_cmd_parser(cmd);
			}
		}
	}
}

void timer_exception_handler_c()
{
	//uart_send_string("In timer interruption\n");
	put32(CORE0_TIMER_IRQ_CTRL, 2);

	unsigned long long cntpct_el0 = 0;//The register count secs with frequency
	asm volatile("mrs %0,cntpct_el0":"=r"(cntpct_el0));
	
	//unsigned long long cntfrq_el0 = 400;//The base frequency
    unsigned long long cntfrq_el0 = 0;//The base frequency
	asm volatile("mrs %0,cntfrq_el0":"=r"(cntfrq_el0));

	unsigned long long sec = cntpct_el0 / cntfrq_el0;
	/*uart_send_string("sec:");
	uart_binary_to_hex(sec);
	uart_send_string("\n");*/
	
	//unsigned long long wait = 800;// wait 2 seconds
    unsigned long long wait = cntfrq_el0 * 2;// wait 2 seconds
	asm volatile ("msr cntp_tval_el0, %0"::"r"(wait));//set new timer
}

void irq_except_handler_c()
{
	uint32 irq_pending1 = get32(IRQ_PENDING_1);
	uint32 core0_interrupt_source = get32(CORE0_INTERRUPT_SOURCE);
	uint32 iir = get32(AUX_MU_IIR_REG);
	

	asm volatile("msr DAIFSet, 0xf"); // Disable interrupts

	if(core0_interrupt_source & CNTPSIRQ_BIT_POSITION) // Timer interrupt
	{
		//disable timer
		asm volatile("msr cntp_ctl_el0, %0"::"r"(0));

		put32(CORE0_TIMER_IRQ_CTRL, 0);

		timer_exception_handler_c();

		//enable timer
		asm volatile("msr cntp_ctl_el0, %0"::"r"(0x1));
	}

	if (irq_pending1 & AUXINIT_BIT_POSTION)
	{
		if(iir & RX_INTERRUPT_ID)
			uart_exception_handler_c();
	}

	asm volatile("msr DAIFClr, 0xf"); // Enable interrupts
}

void except_handler_c()
{
	uart_send_string("In Exception handle\n");
	//read spsr_el1
	unsigned long long spsr_el1 = 0;
	asm volatile("mrs %0, spsr_el1":"=r"(spsr_el1));
	uart_send_string("spsr_el1: ");
	uart_binary_to_hex(spsr_el1);
	uart_send_string("\n");

	//read elr_el1
	unsigned long long elr_el1 = 0;
	asm volatile("mrs %0, elr_el1":"=r"(elr_el1));
	uart_send_string("elr_el1: ");
	uart_binary_to_hex(elr_el1);
	uart_send_string("\n");
	
	//esr_el1
	unsigned long long esr_el1 = 0;
	asm volatile("mrs %0, esr_el1":"=r"(esr_el1));
	uart_binary_to_hex(esr_el1);
	uart_send_string("\n");

	//ec
	unsigned ec = (esr_el1 >> 26) & 0x3F; //0x3F = 0b111111(6)
	uart_send_string("ec: ");
	uart_binary_to_hex(ec);
	uart_send_string("\n");

	while(1){

	}
}