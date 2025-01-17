#ifndef	_MINI_UART_H
#define	_MINI_UART_H

void uart_init ( void );
char uart_recv ( void );
void uart_send ( char c );
void uart_binary_to_hex(unsigned int d);
void uart_send_string(char* str);
void uart_cmd_parser(int cmdNum);
unsigned int uart_cmp_string(char *uart_str, char *cmd_str);
unsigned int uart_get_string_length(char *str1);

/*uart supported cmd*/
void uart_cmd_hello();
void uart_cmd_help();
void uart_cmd_reboot();

#endif  /*_MINI_UART_H */
