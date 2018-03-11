#include <stdio.h>
#include <stdarg.h>

#include <logger.h>
#include <stdio.h>
#include <uart.h>

uint16_t log_level;

void logger_init(uint16_t log_level_)
{
    log_level = log_level_;
}

int logger_(uint16_t level, const char *msg, ...)
{
    va_list ap;
    if (log_level & LOG_PREFIX) {
        switch(level) {
            case LOG_FATAL:     printf("FATAL");     break;
            case LOG_ERROR:     printf("ERROR");     break;
            case LOG_WARNING:   printf("WARNING");   break;
            case LOG_INFO:      printf("INFO");      break;
            case LOG_DEBUG:     printf("DEBUG");     break;
            case LOG_TRACE:     printf("TRACE");     break;
            case LOG_PERIODIC1: printf("PERIODIC1"); break;
            case LOG_PERIODIC2: printf("PERIODIC2"); break;
        }
        printf (": ");
    }
    va_start(ap, msg);
    vprintf(msg, ap);
    va_end(ap);
    return 0;
}

