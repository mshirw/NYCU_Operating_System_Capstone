#ifndef	_MINI_UART_H
#define	_MINI_UART_H

#define CMD_NUM 8

void uart_init ( void );
char uart_recv ( void );
void uart_send ( char c );
void uart_binary_to_hex(unsigned int d);
void uart_send_string(char* str);
void uart_send_string_length(char* str, unsigned int length);
void uart_cmd_parser(int cmdNum);
void uart_enable_interrupt();
unsigned int uart_cmp_string(char *uart_str, char *cmd_str);
unsigned int uart_cmp_string_length(char *uart_str, char *cmd_str, unsigned int length);
unsigned int uart_get_string_length(char *str1);

extern char **cmdArr[CMD_NUM];

/*uart supported cmd*/
void uart_cmd_hello();
void uart_cmd_help();
void uart_cmd_reboot();
void uart_init_cmd();
void printf(char *fmt, ...);

#endif  /*_MINI_UART_H */
