#include <stdint.h>

unsigned int get_RAM_memory_size(void) {
    unsigned int memory_size = 0;
    memory_size = *((unsigned short*)0x413);
    return memory_size;
}

void set_cursor_position(unsigned int x, unsigned int y) {
    unsigned short position = y * 80 + x; // Вычисляем позицию курсора
    outb(VGA_PORT_INDEX, 14); // Устанавливаем индекс верхнего байта
    outb(VGA_PORT_DATA, (position >> 8) & 0xFF); // Устанавливаем верхний байт
    outb(VGA_PORT_INDEX, 15); // Устанавливаем индекс нижнего байта
    outb(VGA_PORT_DATA, position & 0xFF); // Устанавливаем нижний байт
}

void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

void get_cpu_name(char *cpu_name) {
    // Используем CPUID для получения имени процессора
    unsigned int eax, ebx, ecx, edx;

    // Получаем первые три строки имени процессора
    for (eax = 0x80000002; eax <= 0x80000004; eax++) {
        __asm__ __volatile__ (
            "cpuid"
            : "=b"(ebx), "=d"(edx), "=c"(ecx)
            : "a"(eax)
        );
        // Копируем данные в cpu_name
        *(unsigned int*)(cpu_name) = ebx;
        *(unsigned int*)(cpu_name + 4) = edx;
        *(unsigned int*)(cpu_name + 8) = ecx;

        cpu_name += 12; // Переходим к следующему блоку
    }

    // Завершающий нулевой символ
    cpu_name[0] = '\0'; // Устанавливаем нулевой символ в конце
}

