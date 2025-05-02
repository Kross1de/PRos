bits 32
section .text
    ; Multiboot спецификация
    align 4
    dd 0x1BADB002              ; magic
    dd 0x00                    ; flags
    dd - (0x1BADB002 + 0x00)   ; checksum. m+f+c should be zero

global start
global keyboard_handler
global read_port
global write_port
global load_idt
global off

extern kmain                 ; Это определено в C файле
extern keyboard_handler_main

; Чтение из порта
read_port:
    mov edx, [esp + 4]
    in al, dx                ; dx - младшие 16 бит edx
    ret

; Запись в порт
write_port:
    mov edx, [esp + 4]
    mov al, [esp + 8]        ; изменено на [esp + 8] для передачи второго аргумента
    out dx, al  
    ret

; Загрузка таблицы дескрипторов прерываний
load_idt:
    mov edx, [esp + 4]
    lidt [edx]               ; загрузка таблицы дескрипторов прерываний
    sti                      ; включить прерывания
    ret

; Обработчик клавиатуры
keyboard_handler:                 
    call keyboard_handler_main
    iret                     ; возврат из прерывания

; Начало загрузчика
start:
    cli                      ; блокировка прерываний
    mov esp, stack_space     ; установка указателя стека
    ; Инициализация сегментов
    mov ax, 0x0000          ; Установка сегмента данных
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call kmain               ; вызов основной функции ядра
    hlt                      ; остановка процессора
    
off:
   mov ax, 0x5307
   int 0x21
   hlt

section .bss
resb 8192                   ; резервируем 8KB для стека
stack_space:                ; метка для стека

