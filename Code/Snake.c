#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>

const int row = 30;
const int col = 40;
const int snakeMaxLength = 15;
char board[row * col];
struct Food foodCoords[snakeMaxLength];
struct Snake snake;

enum InputDirection{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

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
            printf("%c", board[i * col + j]);
        }
        printf("\n");
    }
}

void randomGenerateFruit(){
    int maxAmountFood = snakeMaxLength;
    int innerCol = col - 2;
    int innerRow = row - 2;

    for(int i = 0; i < maxAmountFood; i++){
        int y = rand() % innerCol + 1;
        int x = rand() % innerRow + 1;

        if(board[y * col + x] == '8'){
            continue;
        }
        board[x * col + y] = '*';
        foodCoords[i].xPos = x;
        foodCoords[i].yPos = y;
    }
}

void generateSnakeHead(){
    int xPos = rand() % (row - 2) + 1;
    int yPos = rand() % (col - 2) + 1;

    board[xPos * col + yPos] = '8';
    snake.xPos = xPos;
    snake.yPos = yPos;
    snake.snakeLength = 1;
}

void movementSnake(){

}

void eatFruit(){

}

void setupGame(){
    initscr();              //Initialize ncurse mode
    raw();                  //no line buffering -> means results will be printed immediately
    noecho();               //disables Copy paste operations and such
    keypad(stdscr, TRUE);   //allows for user keyboard input
    curs_set(0);            //makes cursor invisible (0 = invisible, 1 = visible, 2 = fully visible)
    refresh();              //writes content on screen (terminal) and refreshes
}

int main(void){
    srand(time(NULL));
    setupGame();    

    generateBoard();
    generateSnakeHead();
    randomGenerateFruit();
    printBoard();
}