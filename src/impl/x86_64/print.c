#include "../../intf/print.h"

typedef struct {
    uint8_t character;
    uint8_t color;
} ScreenChar;

ScreenChar* buffer = (ScreenChar*)VIDEO_MEMORY_ADDRESS;

size_t col = 0;
size_t row = 0;
uint8_t color = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;

// TODO: for some reason, the memmove function couldn't be found so I had to implement it here
void* memmove(void* dest, const void* src, size_t n) {
    unsigned char* d = dest;
    const unsigned char* s = src;
    
    if (d < s) {
        // Non-overlapping or source is after destination
        while (n--) {
            *d++ = *s++;
        }
    } else {
        // Overlapping case or source is before destination
        d += n;
        s += n;
        while (n--) {
            *(--d) = *(--s);
        }
    }
    return dest;
}

void clear_row(size_t row) {
    ScreenChar empty;
    empty.character = ' ';
    empty.color = color;

    for (size_t col = 0; col < VIDEO_WIDTH; col++) {
        buffer[col + VIDEO_WIDTH * row] = empty;
    }
}

void print_clear() {
    for (size_t i = 0; i < VIDEO_HEIGHT; i++) {
        clear_row(i);
    }
    col = 0;
    row = 0;  
}

void print_newline() {
    col = 0;

    if (row < VIDEO_HEIGHT - 1) {
        row++;
        return;
    }

    memmove(buffer, buffer + VIDEO_WIDTH, (VIDEO_HEIGHT - 1) * VIDEO_WIDTH * sizeof(ScreenChar));
    clear_row(VIDEO_HEIGHT - 1);
}

void print_char(char character) {
    switch (character) {
        case '\n':
            print_newline();
            break;
        
        case '\t':
            // Handle tab character: move cursor to the next tab stop
            // This is a simple implementation; it might need adjustment for real tab stops.
            col = (col + 8) & ~(8 - 1); // Move to the next multiple of 8
            break;

        default:
            if (col >= VIDEO_WIDTH) {
                print_newline();
            }

            buffer[col + VIDEO_WIDTH * row] = (ScreenChar){
                .character = (uint8_t)character,
                .color = color,
            };

            col++;
            break;
    }
    update_cursor(row, col);
}

void print_str(char* str) {
    if (str == NULL) {
        return;
    }

    for (size_t i = 0; str[i] != '\0'; i++) {
        print_char(str[i]);
    }

    update_cursor(row, col);
}

void print_set_color(uint8_t foreground, uint8_t background) {
    color = (foreground & 0x0F) | ((background & 0x0F) << 4);
}

// Cursor Control

static inline void outb(uint16_t port, uint8_t val) {
    __asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void show_cursor() {
    uint8_t cursor_start;
    
    // Set the cursor scanline start (bit 5 is used to enable/disable the cursor)
    outb(0x3D4, 0x0A);  // Cursor Start register
    cursor_start = inb(0x3D5) & 0xC0;  // Mask off the top 2 bits (preserve them)
    outb(0x3D5, cursor_start & 0xDF);  // Clear the bit that hides the cursor
}

void hide_cursor() {
    uint8_t cursor_start;
    
    // Set the cursor scanline start to disable the cursor (set bit 5)
    outb(0x3D4, 0x0A);  // Cursor Start register
    cursor_start = inb(0x3D5) & 0xC0;  // Mask off the top 2 bits (preserve them)
    outb(0x3D5, cursor_start | 0x20);  // Set bit 5 to disable the cursor
}

void update_cursor(int row, int col) {
    uint16_t pos = row * VIDEO_WIDTH + col;

    // Cursor Location High Byte
    outb(0x3D4, 0x0E);
    outb(0x3D5, (pos >> 8) & 0xFF);

    // Cursor Location Low Byte
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos & 0xFF);
}