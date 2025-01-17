#include "mini_uart.h"
#include "mailbox.h"

void kernel_main(void)
{
	char charBuf[256];
	char *cmdHelp = "# help";
	char *cmdHello = "# hello";
	char *cmdReboot = "# reboot";
	char *cmdGetBoardRevision = "# board revision";
	char *cmdGetARMMemory = "# ARM memory";
	char *cmdPtr;
	char **cmdArr[5] = {cmdHelp, cmdHello, cmdReboot, cmdGetBoardRevision, cmdGetARMMemory};
	int supportCmdNum = 5;

	unsigned int bufIndex = 0;
	uart_init();
	uart_send_string("Hello, world!\r\n");

	while (1) {
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
	}
}
