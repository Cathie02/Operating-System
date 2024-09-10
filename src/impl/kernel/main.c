#include "../../intf/print.h"
#include "../../intf/log.h"
#include "../../intf/memory.h"
#include "../../intf/idt.h"

void kernel_main(){
    print_clear();
    print_set_color(PRINT_COLOR_LIGHT_GREY, PRINT_COLOR_BLACK);
    print_str("Welcome to x64 kernel\n");

    log_message(LOG_LEVEL_DEBUG, "This is a debug message");
    log_message(LOG_LEVEL_INFO, "This is an info message");
    log_message(LOG_LEVEL_WARN, "This is a warning message");
    log_message(LOG_LEVEL_ERROR, "This is an error message");

    print_set_color(PRINT_COLOR_LIGHT_GREY, PRINT_COLOR_BLACK);
    print_str("Initializing memory management...");
    init_memory();
    print_str("Done\n");

    print_str("Initializing IDT...");
    init_idt();
    print_str("Done\n");

    print_str("Enabling interrupts...\n");
    __asm __volatile__("int $0x20");

    while (1) {
        
    }
}