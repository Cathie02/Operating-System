#include "../../intf/idt.h"
#include "../../intf/isr.h"

IDTEntry idt[IDT_SIZE];
IDTPointer idt_pointer;

extern void idt_flush(uint64_t);

void init_idt() {
    idt_pointer.limit = sizeof(IDTEntry) * IDT_SIZE - 1;
    idt_pointer.base = (uint64_t)&idt;

    //memset(&idt, 0, sizeof(IDTEntry) * IDT_SIZE);
    // cant use memset here because it is not implemented yet
    for (int i = 0; i < IDT_SIZE; i++) {
        idt[i].offset_low = 0;
        idt[i].selector = 0;
        idt[i].ist = 0;
        idt[i].type_attr = 0;
        idt[i].offset_mid = 0;
        idt[i].offset_high = 0;
        idt[i].zero = 0;
    }

    // Set up IDT entry for interrupt 32
    set_idt_entry(32, (uint64_t)isr32);

    idt_flush((uint64_t)&idt_pointer);

    // calling int $0x20 here will restart the system
    // __asm __volatile__("int $0x20");
}

void set_idt_entry(int index, uint64_t handler) {

    idt[index].offset_low = handler & 0xFFFF;
    idt[index].selector = 0x08; // Code segment selector
    idt[index].ist = 0;
    idt[index].type_attr = 0x8E; // Present, Ring 0, Interrupt Gate
    idt[index].offset_mid = (handler >> 16) & 0xFFFF;
    idt[index].offset_high = (handler >> 32) & 0xFFFFFFFF;
    idt[index].zero = 0;
}