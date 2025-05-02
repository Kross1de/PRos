#define SIZE 9

void printBoard(char board[]);
bool checkWin(char board[], char player);
bool isBoardFull(char board[]);
int getBestMove(char board[]);
int minimax(char board[], int depth, bool isMaximizing);
void play_tic_tac_toe();

void play_tic_tac_toe() {
    print("Welcome to the Tic - Tac - Toe game\n");
    print("-----------------------------------\n");

    char board[SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
    char player = 'X'; // Игрок
    char bot = 'O'; // Бот

    while (true) {
        clear_screen(); // Очищаем экран перед выводом доски
        printBoard(board);

        // Ход игрока
        print("Enter your move (1-9) or 'q' to quit: ");
        
        char inputBuffer[10]; // Буфер для ввода
        input(inputBuffer, sizeof(inputBuffer)); // Получаем ввод от игрока

        // Проверка, не нажата ли клавиша 'q'
        if (inputBuffer[0] == 'q') {
            break;
        }

        // Преобразуем ввод в число
        int playerMove = atoi(inputBuffer); // Получаем ввод от игрока

        // Проверка на корректность хода
        while (playerMove < 1 || playerMove > 9 || board[playerMove - 1] != ' ') {
            print("Invalid move. Try again.\n");
            input(inputBuffer, sizeof(inputBuffer));
            if (inputBuffer[0] == 'q') {
                print("Thank you for playing!\n");
                return;
            }
            playerMove = atoi(inputBuffer); // Получаем ввод от игрока снова
        }

        board[playerMove - 1] = player;

        if (checkWin(board, player)) {
            printBoard(board);
            print("Congratulations! You win!\n");
            break;
        }

        if (isBoardFull(board)) {
            printBoard(board);
            print("It's a draw!\n");
            break;
        }

        // Ход бота
        int botMove = getBestMove(board);
        board[botMove] = bot;

        if (checkWin(board, bot)) {
            printBoard(board);
            print("Bot wins! Better luck next time.\n");
            break;
        }

        if (isBoardFull(board)) {
            printBoard(board);
            print("It's a draw!\n");
            break;
        }
    }

    print("Thank you for playing!\n");
}

void printBoard(char board[]) {
    print(" ");
    print(board[0] == ' ' ? " " : (char[]){board[0], '\0'});
    print(" | ");
    print(board[1] == ' ' ? " " : (char[]){board[1], '\0'});
    print(" | ");
    print(board[2] == ' ' ? " " : (char[]){board[2], '\0'});
    print("\n");
    print("---|---|---\n");
    print(" ");
    print(board[3] == ' ' ? " " : (char[]){board[3], '\0'});
    print(" | ");
    print(board[4] == ' ' ? " " : (char[]){board[4], '\0'});
    print(" | ");
    print(board[5] == ' ' ? " " : (char[]){board[5], '\0'});
    print("\n");
    print("---|---|---\n");
    print(" ");
    print(board[6] == ' ' ? " " : (char[]){board[6], '\0'});
    print(" | ");
    print(board[7] == ' ' ? " " : (char[]){board[7], '\0'});
    print(" | ");
    print(board[8] == ' ' ? " " : (char[]){board[8], '\0'});
    print("\n");
}

bool checkWin(char board[], char player) {
    return (board[0] == player && board[1] == player && board[2] == player) ||
           (board[3] == player && board[4] == player && board[5] == player) ||
           (board[6] == player && board[7] == player && board[8] == player) ||
           (board[0] == player && board[3] == player && board[6] == player) ||
           (board[1] == player && board[4] == player && board[7] == player) ||
           (board[2] == player && board[5] == player && board[8] == player) ||
           (board[0] == player && board[4] == player && board[8] == player) ||
           (board[2] == player && board[4] == player && board[6] == player);
}

bool isBoardFull(char board[]) {
    for (int i = 0; i < SIZE; i++) {
        if (board[i] == ' ') {
            return false;
        }
    }
    return true;
}

int getBestMove(char board[]) {
    int bestScore = -1000;
    int move = -1;

    for (int i = 0; i < SIZE; i++) {
        if (board[i] == ' ') {
            board[i] = 'O'; // Бот делает ход
            int score = minimax(board, 0, false);
            board[i] = ' '; // Отменяем ход

            if (score > bestScore) {
                bestScore = score;
                move = i;
            }
        }
    }
    return move;
}

int minimax(char board[], int depth, bool isMaximizing) {
    if (checkWin(board, 'O')) return 10 - depth; // Бот выиграл
    if (checkWin(board, 'X')) return depth - 10; // Игрок выиграл
    if (isBoardFull(board)) return 0; // Ничья

    if (isMaximizing) {
        int bestScore = -1000;
        for (int i = 0; i < SIZE; i++) {
            if (board[i] == ' ') {
                board[i] = 'O'; // Бот делает ход
                int score = minimax(board, depth + 1, false);
                board[i] = ' '; // Отменяем ход
                bestScore = (score > bestScore) ? score : bestScore;
            }
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (int i = 0; i < SIZE; i++) {
            if (board[i] == ' ') {
                board[i] = 'X'; // Игрок делает ход
                int score = minimax(board, depth + 1, true);
                board[i] = ' '; // Отменяем ход
                bestScore = (score < bestScore) ? score : bestScore;
            }
        }
        return bestScore;
    }
}

int main() {
    play_tic_tac_toe();
    return 0;
}

