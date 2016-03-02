#ifndef LOGGER_H
#define LOGGER_H

#include <arduino.h>

#define LOG_FATAL            (1 << 0)
#define LOG_ERROR            (1 << 1)
#define LOG_WARNING          (1 << 2)
#define LOG_INFO             (1 << 3)
#define LOG_DEBUG            (1 << 4)
#define LOG_TRACE            (1 << 5)
#define LOG_PERIODIC1        (1 << 6)
#define LOG_PERIODIC2        (1 << 7)

void logger_init(uint16_t log_level_);

#define logger(level,fmt, ...)            \
    do {                                  \
        if (level & log_level) {          \
            printf(fmt, ## __VA_ARGS__);  \
        }                                 \
    }while(0)


extern uint16_t log_level;

#endif

