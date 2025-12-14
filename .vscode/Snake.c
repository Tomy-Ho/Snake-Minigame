#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>
#include <ctype.h>

#define row 30
#define col 40
#define snakeMaxLength 15

struct SnakeParts{
    char snakeParts[snakeMaxLength];
    int x;
    int y;
    int last_x;
    int last_y;
};

struct Snake{
    int snakeLength;
    struct SnakeParts parts[snakeMaxLength];
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
int score = 0;

void generateBoard(){
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            if(i == 0 || i == row - 1){
                board [i * col + j] = '-';
            } else if(j == 0 || j == col - 1){
                board [i * col + j] = '|';
            } else if(board[i * col + j] == '*'){
                continue;
            } else {
                board[i * col + j] = ' ';
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
}

void printGameInfo(){
    mvprintw(row + 1, 0, "Your Score: %d", score);
    mvprintw(row + 2, 0, "Snake growth: %d", snake.snakeLength);
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
}

void initializeSnake(){
    snake.snakeLength = 0;
    for(int i = 0; i < snakeMaxLength; i++){
        snake.parts[i].snakeParts[i] = ' ';
    }
}

void updateSnakePartsPos(int old_x, int old_y){

    for(int i = snakeMaxLength - 1; i > 0; i--){
        if(snake.parts[i].snakeParts[i] == ' '){
            continue;
        }
        snake.parts[i].last_x = snake.parts[i].x;
        snake.parts[i].last_y = snake.parts[i].y;
        snake.parts[i].x = snake.parts[i - 1].x;
        snake.parts[i].y = snake.parts[i - 1].y; 
    }

    snake.parts[0].last_x = snake.parts[0].x;
    snake.parts[0].last_y = snake.parts[0].y;
    snake.parts[0].x = old_x;
    snake.parts[0].y = old_y; 
}

void snakePartsInBoard(){
    int pos_x;
    int pos_y;
    for(int i = 0; i < snakeMaxLength; i++){
        if(snake.parts[i].snakeParts[i] == 'o'){
            pos_x = snake.parts[i].x;
            pos_y = snake.parts[i].y;
            board[pos_y * col + pos_x] = snake.parts[i].snakeParts[i]; 
        }
    }
}

void eatFruit(int position, int old_x, int old_y){
    snake.snakeLength += 1;
    score += 1;

    for(int i = 0; i < snakeMaxLength; i++){
        if(snake.parts[i].snakeParts[i] == ' '){
            snake.parts[i].snakeParts[i] = 'o';
            if(i == 0){
                snake.parts[i].x = old_x;
                snake.parts[i].y = old_y;
            } else {
                snake.parts[i].x = snake.parts[i-1].last_x;
                snake.parts[i].y = snake.parts[i-1].last_y;
            }
            break;
        }
    }
}

void collisionFruit(int *snake_x, int *snake_y, int old_x, int old_y){
    int snakeBoardPos = *snake_y * col + *snake_x;

    if(board[snakeBoardPos] == '*'){
        eatFruit(snakeBoardPos, old_x, old_y);
    }
}

void moveSnake(int *snake_x, int *snake_y, int dx, int dy){
    int old_x = *snake_x;
    int old_y = *snake_y;
    board[old_y * col + old_x] = ' ';

    *snake_x += dx;
    *snake_y += dy;

    collisionFruit(&snake.xPos, &snake.yPos, old_x, old_y);

    if(snake.snakeLength > 0){
        updateSnakePartsPos(old_x, old_y);
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

void quitGame(int inputKey){
    if(tolower(inputKey) == 'q'){
        mvprintw(0, 0, "Game Over.");
        mvprintw(1, 0, "Your score: %d", score);
        gameOver = TRUE;
    }
}

void endGame(){
    mvprintw(row + 2, 0, "Game Over!");
    mvprintw(row + 3, 0, "Your score: %d", score);
    gameOver = TRUE;
}

void selfCollision(int *snake_x, int *snake_y){
    int snakePos = *snake_y * col + *snake_x;

    if(*snake_y < 1 || *snake_x < 1 || *snake_y > row - 2 || *snake_x > col - 2){
        mvprintw(row + 1, 0, "You hit the wall!");
        endGame();
    }

    if(board[snakePos] == 'o'){
        mvprintw(row + 1, 0, "You hit yourself!");
        endGame();
    }
}

void updateBoard(){
    generateBoard();
    board[snake.yPos * col + snake.xPos] = '8';
    snakePartsInBoard();
    printGameInfo();
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
        quitGame(inputKey);
        selfCollision(&snake.xPos, &snake.yPos);
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
    initializeSnake();
    generateSnakeHead();

    runGame();
    return 0;
}