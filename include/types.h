#define VGA_PORT_INDEX 0x3D4
#define VGA_PORT_DATA 0x3D5

#define LINES 25
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE (BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES)
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08
#define ENTER_KEY_CODE 0x1C
#define BACKSPACE_KEY_CODE 0x0E
#define MAX_INPUT_SIZE 256 // Максимальная длина ввода
#define INT_MAX 2147483647
#define INT_MIN -2147483648

// Цвета консольного текста
#define COLOR_BLACK 0x00
#define COLOR_BLUE 0x01
#define COLOR_GREEN 0x02
#define COLOR_CYAN 0x03
#define COLOR_RED 0x04
#define COLOR_MAGENTA 0x05
#define COLOR_BROWN 0x06
#define COLOR_LIGHT_GREY 0x07
#define COLOR_DARK_GREY 0x08
#define COLOR_LIGHT_BLUE 0x09
#define COLOR_LIGHT_GREEN 0x0A
#define COLOR_LIGHT_CYAN 0x0B
#define COLOR_LIGHT_RED 0x0C
#define COLOR_LIGHT_MAGENTA 0x0D
#define COLOR_LIGHT_BROWN 0x0E
#define COLOR_WHITE 0x0F

//файловая система RAM
#define MAX_FILES 100
#define BLOCK_SIZE 512 // Размер блока в байтах
#define MAX_BLOCKS 1024 // Максимальное количество блоков
#define SECTOR_SIZE 512 // Размер сектора в байт
#define DISK_SECTOR_SIZE 512 // Размер сектора в байтах


#define SECTORS_PER_TRACK 63  // Example value, adjust as necessary
#define HEADS 255             // Example value, adjust as necessary
