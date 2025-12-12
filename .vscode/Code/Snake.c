#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>
#include <ctype.h>

const int row = 30;
const int col = 40;
const int snakeMaxLength = 15;

struct Snake{
    int snakeLength;
    int snakeParts[snakeMaxLength];
    int xPos;
    int yPos;
};

struct Food{
    int xPos;
    int yPos;
};

char board[row * col];
struct Food foodCoords[snakeMaxLength];
struct Snake snake;
bool gameOver = FALSE;



void generateBoard(){
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            if(i == 0 || i == row - 1){
                board [i * col + j] = '-';
            } else if(j == 0 || j == col - 1){
                board [i * col + j] = '|';
            } else {
                board [i * col + j] = ' ';
            }
        }
    }
}

void printBoard(){
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            mvprintw(i, j, "%c", board[i * col + j]); //replaced printf with mvprintw from ncurses -> because ncurses takes over the terminal, which broke the gameboard
        }
    }
    mvprintw(row + 1, 0, "Aktuelle Spalten: %d", COLS);
}

void randomGenerateFruit(){
    int maxAmountFood = snakeMaxLength;
    int innerCol = col - 2;
    int innerRow = row - 2;

    for(int i = 0; i < maxAmountFood; i++){
        int x = rand() % innerCol + 1;
        int y = rand() % innerRow + 1;

        if(board[y * col + x] == '8'){
            continue;
        }
        board[y * col + x] = '*';
        foodCoords[i].xPos = x;
        foodCoords[i].yPos = y;
    }
}

void generateSnakeHead(){
    int xPos = rand() % (col - 2) + 1;
    int yPos = rand() % (row - 2) + 1;

    board[yPos * col + xPos] = '8';
    snake.xPos = xPos;
    snake.yPos = yPos;
    snake.snakeLength = 1;
}

void moveSnake(int *snake_x, int *snake_y, int dx, int dy){
    *snake_x += dx;
    *snake_y += dy;

    if(*snake_y < 1){
        *snake_y = 1;
    }

    if(*snake_y > row - 2){
        *snake_y = row - 2;
    }

    if(*snake_x < 1){
        *snake_x = 1;
    }

    if(*snake_x > col - 2){
        *snake_x = col - 2;
    }
}

void movementSnake(int inputKey){
    if(inputKey == ERR){
        return;
    }

    switch(tolower(inputKey)){
        case 'w':
            moveSnake(&snake.xPos, &snake.yPos, 0, -1);
            break;
        case 'a':
            moveSnake(&snake.xPos, &snake.yPos, -1, 0);
            break;
        case 's':
            moveSnake(&snake.xPos, &snake.yPos, 0, 1);
            break;
        case 'd':
            moveSnake(&snake.xPos, &snake.yPos, 1, 0);
            break;
        default:
            break;
    }    
}

void eatFruit(){

}

void updateBoard(){
    generateBoard();
    board[snake.yPos * col + snake.xPos] = '8';
}

void runGame(){
    struct timespec tspec;
    tspec.tv_sec = 0;
    tspec.tv_nsec = 60000;

    while(!gameOver){
        int inputKey = getch();
        movementSnake(inputKey);
        clear();
        updateBoard();
        printBoard();
        refresh();          //writes content on screen (terminal) and refreshes
        nanosleep(&tspec, NULL);
    }
}

void setupGame(){
    initscr();              //Initialize ncurse mode
    raw();                  //no line buffering -> means results will be printed immediately
    noecho();               //disables Copy paste operations and such
    keypad(stdscr, TRUE);   //allows for user keyboard input
    curs_set(0);            //makes cursor invisible (0 = invisible, 1 = visible, 2 = fully visible)
    nodelay(stdscr, TRUE);
}

int main(void){
    srand(time(NULL));
    setupGame();    
    randomGenerateFruit();
    
    generateBoard();
    generateSnakeHead();

    runGame();
    return 0;
}