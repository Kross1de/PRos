#include "include/kernel.h"

void kmain(void) { 
//UI
clear_screen(); 
welcome_screen(); 
print("> Booted successfully!\n> Welcome to the PRoX operation system!\n\n"); 

//используется оперативной памяти...
unsigned int mem_size = get_RAM_memory_size();
print("RAM used: ");
printn(mem_size);
print(" KB\n");

//процессор
char cpu_name[48];
get_cpu_name(cpu_name);
print("Processor: ");
print(cpu_name);

kprint_newline(); 
help(); 

//командная оболочка
commands();

}
