#include "../../intf/log.h"
#include "../../intf/print.h"

uint8_t log_level = LOG_LEVEL_DEBUG;

void log_message(uint8_t level, char* message) {
    if (level < log_level) {
        // We ignore DEBUG messages when log_level is INFO
        return;
    }

    switch (level) {
    case LOG_LEVEL_DEBUG:
        print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
        print_str("[DEBUG] ");
        break;
    case LOG_LEVEL_INFO:
        print_set_color(PRINT_COLOR_GREEN, PRINT_COLOR_BLACK);
        print_str("[INFO] ");
        break;
    case LOG_LEVEL_WARN:
        print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);
        print_str("[WARN] ");
        break;
    case LOG_LEVEL_ERROR:
        print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
        print_str("[ERROR] ");
        break;
    }

    print_str(message);
    print_newline();
}