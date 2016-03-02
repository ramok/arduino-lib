#include <arduino.h>

#include <stdio.h>
#include <util/delay.h>

#include <uart.h>
#include <logger.h>

#define SERIAL_SPEED     115200

int main()
{
    init();
    uart_init(SERIAL_SPEED);
    logger_init(LOG_FATAL|LOG_ERROR|LOG_WARNING|LOG_INFO);

    for (;;)
    {
#ifdef UART_INPUT
        uart_handle();
#endif
        printf("alive\n");
        delay(500);
    }

    return 0;
}

