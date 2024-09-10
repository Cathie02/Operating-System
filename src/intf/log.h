#pragma once

#include <stdint.h>
#include <stddef.h>

enum {
    LOG_LEVEL_DEBUG = 0,
    LOG_LEVEL_INFO = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_ERROR = 3,
};

void log_message(uint8_t level, char* message);