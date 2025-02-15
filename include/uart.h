#ifndef UART_H
#define UART_H

#include <arduino.h>

void uart_init(uint32_t serial_speed, bool empty_line);
void uart_handle();

#ifdef UART_INPUT
#define UART_INPUT_BUF_LEN 30
int uart_char_avail();
int uart_get_line(char *buf);
#endif


#endif

