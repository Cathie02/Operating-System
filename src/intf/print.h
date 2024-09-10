#ifndef DRIVER_VGA_TEXT
#define DRIVER_VGA_TEXT

#include <stdint.h>
#include <stddef.h>

// Printing To Screen: https://wiki.osdev.org/Printing_To_Screen

#define VIDEO_MEMORY_ADDRESS 0xb8000
#define VIDEO_WIDTH 80
#define VIDEO_HEIGHT 25
#define VIDEO_SIZE (VIDEO_WIDTH * VIDEO_HEIGHT)

enum {
    PRINT_COLOR_BLACK = 0,
    PRINT_COLOR_BLUE = 1,
    PRINT_COLOR_GREEN = 2,
    PRINT_COLOR_CYAN = 3,
    PRINT_COLOR_RED = 4,
    PRINT_COLOR_PURPLE = 5,
    PRINT_COLOR_BROWN = 6,
    PRINT_COLOR_LIGHT_GREY = 7,
    PRINT_COLOR_DARK_GREY = 8,
    PRINT_COLOR_LIGHT_BLUE = 9,
    PRINT_COLOR_LIGHT_GREEN = 10,
    PRINT_COLOR_LIGHT_CYAN = 11,
    PRINT_COLOR_LIGHT_RED = 12,
    PRINT_COLOR_LIGHT_PURPLE = 13,
    PRINT_COLOR_YELLOW = 14,
    PRINT_COLOR_WHITE = 15,
};

void print_clear();
void print_char(char character);
void print_str(char* string);
void print_set_color(uint8_t foreground, uint8_t background);

#define CURSOR_PORT_COMMAND 0x3D4
#define CURSOR_PORT_DATA 0x3D5

void show_cursor();
void hide_cursor();
void update_cursor(int row, int col);


#endif