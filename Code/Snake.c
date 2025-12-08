#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int row = 30;
const int col = 40;
char board[row * col];

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
    srand(time(NULL));
    int maxAmountFood = 15;
    int innerCol = col - 1;
    int innerRow = row - 1;

    for(int i = 0; i < maxAmountFood; i++){
        int x = rand() % innerCol + 1;
        int y = rand() % innerRow + 1;

        board[y * col + x] = 'o';
    }
}

int main(void){
    generateBoard();
    randomGenerateFruit();
    printBoard();
}