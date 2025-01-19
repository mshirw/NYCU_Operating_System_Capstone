#include "mini_uart.h"
#include "mailbox.h"
#include "cpio_parser.h"
#include "memalloc.h"

extern char __kernel_start[];
extern char __kernel_end[];

void kernel_main(void)
{
	char charBuf[256];
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
	//volatile const unsigned char *cpio_address = 0x20000000; //raspberry pi 3b+ use this address
	volatile const unsigned char *cpio_address = 0x8000000; //QEMU sim use this address

	unsigned int bufIndex = 0;
	
	//uart_binary_to_hex(__kernel_start);
	//uart_binary_to_hex(__kernel_end);
	
	uart_init();
	uart_send_string("Hello, world!\r\n");
	parse_cpio(cpio_address);
	//must wait all initialization done before enable uart interrupt
	uart_enable_interrupt();

	while(1);

	/*while (1) {
		//wait input and show input char
		charBuf[bufIndex++] = uart_recv();
		uart_send(charBuf[bufIndex-1]);

		if(charBuf[bufIndex-1] == '\r')
		{
			bufIndex = 0;
			uart_send_string("\n");

			for(int cmd = 0; cmd < supportCmdNum; cmd++)
			{
				cmdPtr = cmdArr[cmd];
				if(uart_cmp_string(charBuf, cmdPtr))
				{
					uart_cmd_parser(cmd);
				}
			}
		}
	}*/
}
