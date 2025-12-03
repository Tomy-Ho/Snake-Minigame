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

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            if(!(i == 0 || j == 0 || i == row - 1 || j == col - 1)){
                continue;
            }
            if(maxAmountFood != 0){
                board[(rand() % sizeof(board)) + 1] = 'o';
                maxAmountFood--;
            }
        }
    }
}

int main(void){
    generateBoard();
    randomGenerateFruit();
    printBoard();
}