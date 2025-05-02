void calculator(void) {
    color(COLOR_GREEN);
    print("\nCALCULATOR");
    kprint_newline();

    float num;
    float num2;

    print("Please enter first num: ");
    num = inputf(); // Ввод числа с плавающей точкой

    print("Please enter second num: ");
    num2 = inputf(); // Ввод числа с плавающей точкой

    float result = num + num2;
    float result2 = num - num2;
    float result3 = num * num2;

    print_float(num); // Используем новую функцию для вывода float
    print(" + ");
    print_float(num2);
    print(" = ");
    print_float(result);
    print("\n");

    print_float(num);
    print(" - ");
    print_float(num2);
    print(" = ");
    print_float(result2);
    print("\n");

    print_float(num);
    print(" * ");
    print_float(num2);
    print(" = ");
    print_float(result3);
    print("\n");

    if (num2 != 0) {
        float result4 = num / num2;
        print_float(num);
        print(" / ");
        print_float(num2);
        print(" = ");
        print_float(result4);
        print("\n");
    } else {
        color(COLOR_RED);
        print("pros error: division by zero");
        color(COLOR_GREEN);
    }
}

unsigned long factorial(int n) {
    if (n < 0) return 0; // Факториал для отрицательных чисел не определен
    unsigned long result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

void factorial_calculator(void) {
    color(COLOR_GREEN);
    print("\nFACTORIAL CALCULATOR");
    kprint_newline();
    int num;
    print("Please enter a number:");
    num = inputn();
    unsigned long result = factorial(num);
    printn(num);
    print("! = ");
    printn(result);
    print("\n");
    kprint_newline();
}

void decimal_to_binary(int n) {
    if (n == 0) {
        print("0");
        return;
    }
    char binary[32];
    int index = 0;
    while (n > 0) {
        binary[index++] = (n % 2) + '0';
        n /= 2;
    }
    for (int i = index - 1; i >= 0; i--) {
        vidptr[current_loc * 2] = binary[i];
        vidptr[current_loc * 2 + 1] = current_color; // Используем текущий цвет
        current_loc++;
    }
}

void binary_converter(void) {
    color(COLOR_GREEN);
    print("\nDECIMAL TO BINARY CONVERTER");
    kprint_newline();
    int num;
    print("Please enter a number:");
    num = inputn();
    print("Binary representation of ");
    printn(num);
    print(" is: ");
    decimal_to_binary(num);
    print("\n");
    kprint_newline();
}

int pongmain() {
    int ballX = 40, ballY = 13; // Увеличено на 1
    int vectorBallX = 1, vectorBallY = 0;
    int leftPosY = 13, rightPosY = 13; // Увеличено на 1
    const int leftPosX = 10, rightPosX = 70;
    int pointL = 0, pointR = 0;

    // Переменные для управления скоростью
    unsigned int paddleSpeedDelayLeft = 100000; // Задержка для левой ракетки
    unsigned int paddleSpeedDelayRight = 1100000; // Задержка для правой ракетки
    unsigned int ballSpeedDelay = 10000000;  // Задержка для мяча

    clear_screen();

    while (1) {
        // Проверяем ввод с клавиатуры
        char inputBuffer[2] = {0};
        unsigned char status = read_port(KEYBOARD_STATUS_PORT);
        if (status & 0x01) {
            unsigned char keycode = read_port(KEYBOARD_DATA_PORT);
            if (keycode == ENTER_KEY_CODE) {
                kprint_newline();
            } else if (keycode == BACKSPACE_KEY_CODE) {
                if (current_loc > 0) {
                    current_loc--;
                    vidptr[current_loc * 2] = ' ';
                    vidptr[current_loc * 2 + 1] = current_color; // Используем текущий цвет
                }
            } else if (keycode < 128) {
                char c = keyboard_map[keycode];
                if (c != 0) {
                    inputBuffer[0] = c;  // Сохраняем считанный символ
                    if (c == 'w' && leftPosY > 1) {
                        leftPosY--;
                        delay(paddleSpeedDelayLeft);  // Задержка для левой ракетки
                    } else if (c == 's' && leftPosY < 24) { // Изменено на 24
                        leftPosY++;
                        delay(paddleSpeedDelayLeft);  // Задержка для левой ракетки
                    }
                    else if (c == 'q') { 
                        break;
                    }
                }
            }
        }

        // Управляем правой ракеткой (бот)
        if (ballY < rightPosY && rightPosY > 1) {
            rightPosY--;
            delay(paddleSpeedDelayRight);  // Задержка для правой ракетки
        } else if (ballY > rightPosY + 1 && rightPosY < 24) { // Изменено на 24
            rightPosY++;
            delay(paddleSpeedDelayRight);  // Задержка для правой ракетки
        }

        // Двигаем мяч
        ballX += vectorBallX;
        ballY += vectorBallY;

        // Проверяем столкновение с верхней и нижней границами
        if (ballY <= 1 || ballY >= 25) { // Изменено на 25
            vectorBallY = -vectorBallY;
        }

       // Проверяем столкновение с ракетками
if (ballX == leftPosX + 1 && (ballY == leftPosY || ballY == leftPosY + 1 || ballY == leftPosY - 1)) {
    vectorBallX = -vectorBallX;
    vectorBallY = (rand() % 3) - 1; // Устанавливаем случайное направление по Y
} else if (ballX == rightPosX - 1 && (ballY == rightPosY || ballY == rightPosY + 1 || ballY == rightPosY - 1)) {
    vectorBallX = -vectorBallX;
    vectorBallY = (rand() % 3) - 1; // Устанавливаем случайное направление по Y
}


        // Проверяем выход мяча за пределы
        if (ballX <= 1) {
            pointR++;
            ballX = 40; ballY = 13; // Ставим мяч на старт, увеличено на 1
            vectorBallX = 1; // Сбрасываем направление
            vectorBallY = 0; // Сбрасываем направление
        } else if (ballX >= 78) {
            pointL++;
            ballX = 40; ballY = 13; // Ставим мяч на старт, увеличено на 1
            vectorBallX = -1; // Сбрасываем направление
            vectorBallY = 0; // Сбрасываем направление
        }

        // Отрисовка
        clear_screen();
        for (int high = 0; high < LINES; high++) { // Изменено на LINES
            for (int width = 0; width < 80; width++) {
                if (high == 0 || high == 26) { // Изменено на 26
                    vidptr[high * COLUMNS_IN_LINE * 2 + width * 2] = '-';
                } else if ((high > 0 && high < 26) && (width == 0 || width == 79)) { // Изменено на 26
                    vidptr[high * COLUMNS_IN_LINE * 2 + width * 2] = '|';
                } else if (high == ballY && width == ballX) {
                    vidptr[high * COLUMNS_IN_LINE * 2 + width * 2] = '*';
                } else if ((high == leftPosY && width == leftPosX) || 
                           (high + 1 == leftPosY && width == leftPosX) || 
                           (high - 1 == leftPosY && width == leftPosX)) {
                    vidptr[high * COLUMNS_IN_LINE * 2 + width * 2] = '#';
                } else if ((high == rightPosY && width == rightPosX) || 
                           (high + 1 == rightPosY && width == rightPosX) || 
                           (high - 1 == rightPosY && width == rightPosX)) {
                    vidptr[high * COLUMNS_IN_LINE * 2 + width * 2] = '#';
                } else {
                    vidptr[high * COLUMNS_IN_LINE * 2 + width * 2] = ' ';
                }
                vidptr[high * COLUMNS_IN_LINE * 2 + width * 2 + 1] = current_color; // Используем текущий цвет
            }
        }
        print("Score: Player: ");
        printn(pointL);
        print(" Bot: ");
        printn(pointR);
        
        // Задержка для управления скоростью игры
        delay(ballSpeedDelay); 
    }

    return 0;
}

void guess_number_game(void) {
    clear_screen();
    color(COLOR_GREEN);
    print("\nWELCOME TO THE GUESS NUMBER GAME");
    kprint_newline();
    
    int number_to_guess = rand() % 100 + 1; // Генерируем случайное число от 1 до 100
    int guessed_number;
    int attempts = 0;

    print("I have selected a number between 1 and 100. Try to guess it!\n");

    while (1) {
        print("Enter your guess: ");
        guessed_number = inputn(); // Получаем число от пользователя
        attempts++;

        if (guessed_number < number_to_guess) {
            print("Too low! Try again.\n");
        } else if (guessed_number > number_to_guess) {
            print("Too high! Try again.\n");
        } else {
            print("Congratulations! You've guessed the number ");
            printn(number_to_guess);
            print(" in ");
            printn(attempts);
            print(" attempts!\n");
            break;
        }
    }
    kprint_newline();
}

#include "tic-tac-toe.h"
