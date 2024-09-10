#include "../../intf/print.h"
#include "../../intf/isr.h"

void isr32() {
    __asm volatile ("cli; hlt");
    print_str("Interrupt 32 triggered!\n");
}