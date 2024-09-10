#include "../../intf/memory.h"

// Writing a memory manager: https://wiki.osdev.org/Writing_a_memory_manager

#define PAGE_SIZE 4096
#define MEM_SIZE 1024 * 1024 * 1024 // 1GB

uint8_t memory_bitmap[MEM_SIZE / PAGE_SIZE / 8];

static void mark_page_used(size_t page) {
    memory_bitmap[page / 8] |= 1 << (page % 8);
}

static void mark_page_free(size_t page) {
    memory_bitmap[page / 8] &= ~(1 << (page % 8));
}

static size_t find_free_page() {
    for (size_t byte = 0; byte < sizeof(memory_bitmap); byte++) {
        if (memory_bitmap[byte] != 0xFF) {  // Check if any bit is free
            for (size_t bit = 0; bit < 8; bit++) {
                if (!(memory_bitmap[byte] & (1 << bit))) {
                    return byte * 8 + bit;  // Return the free page index
                }
            }
        }
    }
    return (size_t)-1;  // No free pages found
}

void init_memory() {
    // Clear the memory bitmap (mark all pages as free)
    for (size_t i = 0; i < sizeof(memory_bitmap); i++) {
        memory_bitmap[i] = 0; // All bits 0 (free)
    }

    // Reserve the first few pages (e.g., kernel or hardware reserved memory)
    for (size_t i = 0; i < 256; i++) {  // Reserve the first 1MB of memory
        mark_page_used(i);
    }
}

void* allocate_page() { // similar to malloc()
    size_t page = find_free_page();
    if (page == (size_t)-1) {
        return NULL;  // No free pages available
    }
    mark_page_used(page);
    return (void*)(page * PAGE_SIZE);
}

void free_page(void* ptr) { // similar to free()
    size_t page = (size_t)ptr / PAGE_SIZE;
    mark_page_free(page);
}