#pragma once

#include <stdint.h>
#include <stddef.h>

// basic memory management

void init_memory(); // initialize memory management
void* allocate_page(); // allocate a page
void free_page(void* ptr); // free a page
