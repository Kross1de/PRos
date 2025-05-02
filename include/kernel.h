#include "keyboard_map.h"
#include "types.h"
#include "externes.h"
#include "PC.h"
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>


unsigned int current_loc = 0;
char *vidptr = (char*)0xb8000;
unsigned int lines = 0; // Переменная для отслеживания количества строк
unsigned char current_color = 0x07; // Устанавливаем начальный цвет текста

struct IDT_entry {
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];

int atoi(const char *str) {
    int result = 0;
    int sign = 1;

    while (*str == ' ') str++;

    if (*str == '-' || *str == '+') {
        if (*str == '-') sign = -1;
        str++;
    }

    while (*str >= '0' && *str <= '9') {
        int digit = *str - '0';
        if (result > (INT_MAX - digit) / 10) {
            return (sign == 1) ? INT_MAX : INT_MIN;
        }
        result = result * 10 + digit;
        str++;
    }

    return sign * result;
}

float atof(const char *str) {
    float result = 0.0;
    float factor = 1.0;
    int decimal_place = 0;
    int sign = 1;

    while (*str == ' ') str++; // Пропускаем пробелы

    if (*str == '-' || *str == '+') {
        if (*str == '-') sign = -1;
        str++;
    }

    // Чтение целой части
    while (*str >= '0' && *str <= '9') {
        result = result * 10.0 + (*str - '0');
        str++;
    }

    // Чтение дробной части
    if (*str == '.') {
        str++;
        while (*str >= '0' && *str <= '9') {
            result = result * 10.0 + (*str - '0');
            decimal_place++;
            str++;
        }
    }

    // Двигаем десятичную точку влево
    while (decimal_place--) {
        result /= 10.0;
    }

    return sign * result;
}

void print(const char *str) {
    unsigned int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n') {
            current_loc += 80 - (current_loc % 80);
            lines++; // Увеличиваем количество строк
        } else {
            vidptr[current_loc * 2] = str[i];
            vidptr[current_loc * 2 + 1] = current_color; // Используем текущий цвет
            current_loc++;
        }

        if (current_loc >= 80 * 25) {
            current_loc = 0;
        }
        i++;
    }

    // Проверяем количество строк
    if (lines >= LINES) {
        clear_screen(); // Очищаем экран, если строк больше 0
        lines = 0; // Сбрасываем счетчик строк
    }
}

void printn(int num) {
    char buffer[32];
    int i = 0, isNegative = 0;

    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    do {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    } while (num > 0);

    if (isNegative) {
        buffer[i++] = '-';
    }

    // Обратный порядок
    for (int j = i - 1; j >= 0; j--) {
        vidptr[current_loc * 2] = buffer[j];
        vidptr[current_loc * 2 + 1] = current_color; // Используем текущий цвет
        current_loc++;

        if (current_loc >= 80 * 25) {
            current_loc = 0;
        }
    }
    
    // Проверяем количество строк
    if (lines >= LINES) {
        clear_screen(); // Очищаем экран, если строк больше 0
        lines = 0; // Сбрасываем счетчик строк
    }
}

void print_float(float num) {
    // Функция для вывода числа с плавающей точкой
    char buffer[32];
    int integer_part = (int)num; // Получаем целую часть
    float fractional_part = num - integer_part; // Получаем дробную часть
    int i = 0;

    // Печатаем целую часть
    printn(integer_part);

    // Печатаем дробную часть
    if (fractional_part != 0.0) {
        print(".");
        fractional_part *= 10; // Умножаем на 10 для получения первой цифры дробной части
        while (fractional_part > 0.00001) { // Печатаем до 5 знаков после запятой
            int digit = (int)fractional_part; // Получаем цифру
            printn(digit);
            fractional_part = (fractional_part - digit) * 10; // Убираем целую часть и умножаем на 10
        }
    }
}

void kprint_newline(void) {
    current_loc += (80 - (current_loc % 80));
    lines++; // Увеличиваем количество строк
}

void clear_screen(void) {
    unsigned int i = 0;
    while (i < SCREENSIZE) {
        vidptr[i++] = ' ';
        vidptr[i++] = current_color; // Используем текущий цвет
    }
    current_loc = 0;
    lines = 0; // Сбрасываем количество строк
    set_cursor_position(0, 0);
}

void keyboard_handler_main(void) {
    unsigned char status;
    char keycode;

    write_port(0x20, 0x20);
    status = read_port(KEYBOARD_STATUS_PORT);

    if (status & 0x01) {
        keycode = read_port(KEYBOARD_DATA_PORT);
        if (keycode < 0) return;
        if (keycode == ENTER_KEY_CODE) {
            kprint_newline();
            return;
        }
        if (keycode == BACKSPACE_KEY_CODE) {
            if (current_loc > 0) {
                current_loc--;
                vidptr[current_loc * 2] = ' ';
                vidptr[current_loc * 2 + 1] = current_color; // Используем текущий цвет
            }
            return;
        }
        if (keycode < 128) {
            char c = keyboard_map[keycode];
            if (c != 0) {
                vidptr[current_loc * 2] = c;
                vidptr[current_loc * 2 + 1] = current_color; // Используем текущий цвет
                current_loc++;
            }
        }
    }
}

void input(char *buffer, int max_size) {
    unsigned int index = 0;
    while (index < max_size - 1) {
        unsigned char status = read_port(KEYBOARD_STATUS_PORT);
        if (status & 0x01) {
            unsigned char keycode = read_port(KEYBOARD_DATA_PORT);
            if (keycode == ENTER_KEY_CODE) {
                break;
            }
            if (keycode < 128) {
                char c = keyboard_map[keycode];
                if (c != 0) {
                    if (c == '\b') {
                        if (index > 0) {
                            index--;
                            current_loc--;
                            vidptr[current_loc * 2] = ' ';
                            vidptr[current_loc * 2 + 1] = current_color; // Используем текущий цвет
                            set_cursor_position(current_loc % 80, current_loc / 80); // Обновляем курсор
                        }
                    } else {
                        buffer[index++] = c;
                        vidptr[current_loc * 2] = c;
                        vidptr[current_loc * 2 + 1] = current_color; // Используем текущий цвет
                        current_loc++;
                        set_cursor_position(current_loc % 80, current_loc / 80); // Обновляем курсор
                    }
                }
            }
        }
    }
    buffer[index] = '\0';
    kprint_newline();
}

int inputn() {
    char buffer[MAX_INPUT_SIZE];
    input(buffer, MAX_INPUT_SIZE);
    return atoi(buffer);
}

float inputf() {
    char buffer[MAX_INPUT_SIZE];
    input(buffer, MAX_INPUT_SIZE);
    return atof(buffer);
}

void idt_init(void) {
    unsigned long keyboard_address;
    unsigned long idt_address;
    unsigned long idt_ptr[2];

    keyboard_address = (unsigned long)keyboard_handler;
    IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
    IDT[0x21].selector = KERNEL_CODE_SEGMENT_OFFSET;
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = INTERRUPT_GATE;
    IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

    write_port(0x20, 0x11);
    write_port(0xA0, 0x11);
    write_port(0x21, 0x20);
    write_port(0xA1, 0x28);
    write_port(0x21, 0x00);
    write_port(0xA1, 0x00);
    write_port(0x21, 0x01);
    write_port(0xA1, 0x01);

    write_port(0x21, 0xff);
    write_port(0xA1, 0xff);

    idt_address = (unsigned long)IDT;
    idt_ptr[0] = (sizeof(struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16;
    load_idt(idt_ptr);
}

void kb_init(void) {
    write_port(0x21, 0xFD);
}

void welcome_screen(void){
    color(COLOR_WHITE);
    print("                               PROX DEVELOPMENT\n");
    print("                               ________________\n\n");
    print("                         #####~~#####~~~####~~~####~");
    kprint_newline();
    print("                         ##~~##~##~~##~##~~##~##~~~~");
    kprint_newline();
    print("                         #####~~#####~~##~~##~~####~");
    kprint_newline();
    print("                         ##~~~~~##~~##~##~~##~~~~~##");
    kprint_newline();
    print("                         ##~~~~~##~~##~~####~~~####~\n\n");
    kprint_newline();
    color(COLOR_WHITE);
    print("                         PROS version -1.4\n");
    print("                         prox kernel version -1.1\n");
    print("                         Creator -PRoX <Faddey Kabanov>\n\n");
    print("                         Here is a simple operation system. \n                         It using prox kernel.\n\n");
    print("                         press any key to start PROS...\n");
    
    while (1) {
        unsigned char status = read_port(KEYBOARD_STATUS_PORT);
        if (status & 0x01) {
            read_port(KEYBOARD_DATA_PORT);
            break;
        }
    }

    clear_screen();
}


void OSinfo(void){
    color(COLOR_CYAN);
    print("\nABOUT OS");
    kprint_newline();
    
    print("#####~~#####~~~####~~~####~");
    kprint_newline();
    print("##~~##~##~~##~##~~##~##~~~~");
    kprint_newline();
    print("#####~~#####~~##~~##~~####~");
    kprint_newline();
    print("##~~~~~##~~##~##~~##~~~~~##");
    kprint_newline();
    print("##~~~~~##~~##~~####~~~####~\n");
    kprint_newline();
    
    print("OS kernel: prox kernel");
    kprint_newline();
    
    print("prox kernel version: 1.1");
    kprint_newline();
    
    print("OS name: PRos");
    kprint_newline();
    
    print("OS creator: PRoX <Faddey Kabanov>");
    kprint_newline();
    
    print("CPU support: i think all");
    kprint_newline();
    color(COLOR_GREEN);
}

void help(void) {
    color(COLOR_GREEN);
    print("\nHELP");
    kprint_newline();
    kprint_newline();
    
    print("Type 'osinfo' to get information about PRos");
    kprint_newline();
    print("Type 'cls' to clear terminal");
    kprint_newline();
    print("Type 'help' to print help");
    kprint_newline();
    print("Type 'off' to power off the PC");
    kprint_newline();
    print("Type 'calculator' to start calculator");
    kprint_newline();
    print("Type 'factorial' to calculate factorial");
    kprint_newline();
    print("Type 'pong' to start pong game");
    kprint_newline();
    print("Type 'guess' to start guess my number game");
    kprint_newline();
    print("Type 'tic-tac-toe' to start tic-tac-toe game");
    kprint_newline();
    print("Type 'ram' to see, how many RAM is using for files in RAM_FS");
    kprint_newline();
    print("Type 'binary' to convert decimal to binary");
    print("\n");
    kprint_newline(); 
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

void commands(void) {
    color(COLOR_GREEN);
    char buffer[MAX_INPUT_SIZE];
    print("> ");
    input(buffer, MAX_INPUT_SIZE);

    while (1) {
        if (strcmp(buffer, "calculator") == 0) {
            calculator();
        } 
        else if (strcmp(buffer, "factorial") == 0) {
            factorial_calculator();
        }
        else if (strcmp(buffer, "osinfo") == 0) {
            OSinfo();
        }
        else if (strcmp(buffer, "binary") == 0) {
            binary_converter();
        }
        else if (strcmp(buffer, "help") == 0) {
            help();
        }
        else if (strcmp(buffer, "off") == 0) {
            off();
        }
        else if (strcmp(buffer, "cls") == 0) {
            clear_screen();
        } 
         else if (strcmp(buffer, "ram") == 0) {
            unsigned int mem_size = get_RAM_memory_size();
            print("RAM used: ");
            printn(mem_size);
            print(" KB\n");
        }
        else if (strcmp(buffer, "guess") == 0) {
            guess_number_game();
        }
        else if (strcmp(buffer, "pong") == 0) {
            pongmain();
            clear_screen();
        } 
        else if (strcmp(buffer, "tic-tac-toe") == 0) {
            play_tic_tac_toe();
        } else {
            color(COLOR_RED);
            print("pros error: unknown command");
            color(COLOR_GREEN);
            kprint_newline();
        }
        print("> ");
        input(buffer, MAX_INPUT_SIZE);
    }
}

// Рандом
unsigned int rand() {
    static unsigned int seed = 12345; // Начальное значение
    seed = (seed * 1103515245 + 12345) & 0x7fffffff; // Обновляем seed
    return seed; // Возвращаем случайное число
}

// Задержка
void delay(unsigned int count) {
    for (volatile unsigned int i = 0; i < count; i++);
}

// Задать цвет текста в консоли
void color(unsigned char text_color) {
    if (text_color > 15) {
        return; 
    }

    current_color = text_color; // Устанавливаем цвет текста
}

void bgcolor(unsigned char bg_color) {
    // Проверяем, что цвет фона находится в допустимых пределах (0-15)
    if (bg_color > 15) {
        return; 
    }

    // Обновляем цвет фона для всех символов на экране
    for (unsigned int i = 0; i < SCREENSIZE; i += 2) {
        // Устанавливаем фон в старший ниббл
        vidptr[i + 1] = (vidptr[i + 1] & 0x0F) | (bg_color << 4); 
    }
}

void draw_square(int x, int y, int size, unsigned char color) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (x + j < COLUMNS_IN_LINE && y + i < LINES && x + j >= 0 && y + i >= 0) {
                vidptr[(y + i) * COLUMNS_IN_LINE * 2 + (x + j) * 2] = '#'; // Символ для рисования
                vidptr[(y + i) * COLUMNS_IN_LINE * 2 + (x + j) * 2 + 1] = color; // Цвет
            }
        }
    }
}


// Реализация memset
void* memset(void* s, int c, size_t n) {
    unsigned char* p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

// Реализация memcpy
void* memcpy(void* dest, const void* src, size_t n) {
    unsigned char* d = dest;
    const unsigned char* s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

// Реализация strcpy
char* strcpy(char* dest, const char* src) {
    char* ptr = dest;
    while ((*ptr++ = *src++));
    return dest;
}

// Реализация strlen
size_t strlen(const char* s) {
    const char* p = s;
    while (*p) {
        p++;
    }
    return p - s;
}

#include "programms.h"
