#include <iostream>
#include "board.h"

char** createBoard(int width, int height){
    char** board = new char* [height];
    for(int i = 0; i < height; i++){
        board[i] = new char [width];
    }

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            board[i][j] = ' ';
        }
    }

    return board;
}

void destroyBoard(char** board, int height){
    for(int i = 0; i < height; i++){
        delete[] board[i];
    }

    delete[] board;
}

void displayBoard(char** board, int width, int height){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            std::cout << "+---";
        }
        std::cout << "+" << std::endl;

        for(int j = 0; j < width; j++){
            std::cout << "| " << board[i][j] << " ";
        }
        std::cout << "|" << std::endl;
    }

    for(int j = 0; j < width; j++){
        std::cout << "+---";
    }
    std::cout << "+" << std::endl;
}

int checkVerticalAxis(char** board, int width, int height, char character, int count){
    for(int i = 0; i < width; i++){
        int row = 1;
        int temp = 0;

        for(int j = 0; j < height; j++){
            if(board[j][i] == character){
                temp++;
                row = 1;
            }
            else
                row = 0;
    
            if(temp == count)
                return 1;

            if(row == 0)
                temp = 0;
        } 
    }

    return 0;
}

int checkHorizontalAxis(char** board, int width, int height, char character, int count){
    for(int i = 0; i < height; i++){
        int row = 1;
        int temp = 0;

        for(int j = 0; j < width; j++){
            if(board[i][j] == character){
                temp++;
                row = 1;
            }
            else
                row = 0;
    
            if(temp == count)
                return 1;

            if(row == 0)
                temp = 0;
        } 
    }

    return 0;
}

int checkDiagonalULDR(char** board, int width, int height, char character, int count){
    int done = 0;
    int x = width - 1, y = 0;

    while(!done){
        int x2 = x - 1, y2 = y - 1;

        int row = 1;
        int temp = 0;

        do{
            x2++;
            y2++;
            
            if(board[y2][x2] == character){
                temp++;
                row = 1;
            }
            else
                row = 0;

            if(temp == count)
                return 1;

            if(row == 0)
                temp = 0;
        } while(x2 != width - 1 && y2 != height - 1);

        y = x > 0 ? 0 : y + 1;
        x = x > 0 ? x - 1 : 0;

        if(x == 0 && y == height - 1){
            done = 1;
        }  
    }

    return 0;
}

int checkDiagonalDLUR(char** board, int width, int height, char character, int count){
    int done = 0;
    int x = 0, y = 0;

    while(!done){
        int x2 = x - 1, y2 = y + 1;

        int row = 1;
        int temp = 0;

        do{
            x2++;
            y2--;
            
            if(board[y2][x2] == character){
                temp++;
                row = 1;
            }
            else
                row = 0;

            if(temp == count)
                return 1;

            if(row == 0)
                temp = 0;
        } while(x2 != width - 1 && y2 != 0);

        x = y < height - 1 ? 0 : x + 1;
        y = y < height - 1 ? y + 1 : y;

        if(x == width - 1 && y == height - 1){
            done = 1;
        }  
    }

    return 0;
}

int checkForDraw(char** board, int width, int height){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(board[i][j] == ' ')
                return 0;
        }
    }

    return 1;
}
