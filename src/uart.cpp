#include <uart.h>

#include <stdio.h>

static FILE uart_io = {0};

#ifdef UART_INPUT 
static char uart_rx_buf[UART_INPUT_BUF_LEN + 1];
static int  uart_rx_buf_len = 0;
static bool uart_rx_buf_ready = false;
static bool uart_rx_empty_line = false;

static int uart_getchar(FILE *stream)
{
    return Serial.read();
}

int uart_char_avail() {
    return Serial.available();
}

void uart_handle()
{
    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\r' || c == '\n') {
            uart_rx_buf[uart_rx_buf_len] = 0;
            if (uart_rx_buf_len != 0 || uart_rx_empty_line)
                uart_rx_buf_ready = true;
#ifdef UART_INPUT_EDIT
            Serial.write("\r\n");
#endif
        } else {
            if (uart_rx_buf_len < UART_INPUT_BUF_LEN) {
                switch (c) {
                    case 0x7f: /* backspace */
                        if (uart_rx_buf_len > 0) {
                            uart_rx_buf_len--;
#ifdef UART_INPUT_EDIT
                            Serial.write("\b \b");
#endif
                        }
                        break;
                    default:
                        uart_rx_buf[uart_rx_buf_len++] = c;
#ifdef UART_INPUT_EDIT
                        Serial.write(c) ;
#endif
                        break;
                }
            }
        }
    }
}

int uart_get_line(char *buf)
{
    int rc = -1;
    if (uart_rx_buf_ready) {
        memcpy(buf, uart_rx_buf, uart_rx_buf_len);
        buf[uart_rx_buf_len] = 0;
        rc = uart_rx_buf_len;
        uart_rx_buf_len = 0;

        uart_rx_buf_ready = false;
    }
    return rc;
}
#else /* UART_INPUT */
static int (*uart_getchar)(FILE *stream) = NULL;

int uart_char_avail() {
    return 0;
}

int uart_get_line(char *buf) {
    buf = buf;
    return 0;
}

void uart_handle()
{

}
#endif /* UART_INPUT */

static int uart_putchar(char c, FILE *stream)
{
    if (c == '\n')
        Serial.write('\r') ;
    Serial.write(c) ;
    return 0 ;
}

void uart_init(uint32_t serial_speed, bool empty_line = false)
{
    Serial.begin(serial_speed);
    fdev_setup_stream (&uart_io, uart_putchar, uart_getchar, _FDEV_SETUP_RW);
    stderr = stdout = stdin = &uart_io;
    uart_rx_empty_line = empty_line;
}


