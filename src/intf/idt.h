// idt x86_64 interrupt descriptor table

#ifndef _IDT_H
#define _IDT_H

#include <stdint.h>

#define IDT_SIZE 256

typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t type_attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t zero;
} IDTEntry;

typedef struct {
    uint16_t limit;
    uint64_t base;
} IDTPointer;

void init_idt();
void set_idt_entry(int index, uint64_t handler);

#endif