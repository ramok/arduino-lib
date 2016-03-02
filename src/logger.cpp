#include <logger.h>
#include <stdio.h>
#include <uart.h>

uint16_t log_level;

void logger_init(uint16_t log_level_)
{
    log_level = log_level_;
}

