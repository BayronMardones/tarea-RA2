#include "game.h"
#include <iostream>

Game::Game() : board(6, std::vector<char>(7, ' ')) {} //Inicializa el tablero

void Game::printBoard() { //Imprime el tablero
    for (const auto& row : board) {
        for (const auto& cell : row) {
            std::cout << (cell == ' ' ? '.' : cell) << " "; //Cambiar '.' por ' ' para que se vea mejor
        }
        std::cout << std::endl;
    }
    std::cout << "1 2 3 4 5 6 7" << std::endl; //Agregar números de columna
}

bool Game::dropPiece(int col, char piece) { //Coloca una pieza en la columna col
    if (col < 1 || col > 7 || board[0][col-1] != ' ') { //Agregar condición para que no se pueda colocar fuera del tablero
        return false;
    }
    for (int i = 5; i >= 0; --i) { //Agregar condición para que no se pueda colocar en una columna llena
        if (board[i][col-1] == ' ') {
            board[i][col-1] = piece;
            return true;
        }
    }
    return false;
}

bool Game::checkWin(char piece) { //Revisa si hay un ganador
    return checkHorizontal(piece) || checkVertical(piece) || checkDiagonal(piece);
}

bool Game::isFull() { //Revisa si el tablero está lleno
    for (const auto& row : board) {
        for (const auto& cell : row) {
            if (cell == ' ') {
                return false; //Agregar condición para que no se pueda llenar el tablero si hay un ganador
            }
        }
    }
    return true; //Todas las celdas estan llenas
}

bool Game::checkHorizontal(char piece) { //Revisa si hay 4 en línea horizontal
    for (const auto& row : board) {
        for (int col = 0; col < 4; ++col) {
            if (row[col] == piece && row[col+1] == piece && row[col+2] == piece && row[col+3] == piece) { 
                return true; //Verifica 4 piezas consecutivas horizontales
            }
        }
    }
    return false;
}

bool Game::checkVertical(char piece) { //Revisa si hay 4 en línea vertical
    for (int col = 0; col < 7; ++col) {
        for (int row = 0; row < 3; ++row) {
            if (board[row][col] == piece && board[row+1][col] == piece && board[row+2][col] == piece && board[row+3][col] == piece) {
                return true; //Verifica 4 piezas consecutivas verticales
            }
        }
    }
    return false;
}

bool Game::checkDiagonal(char piece) { //Revisa si hay 4 en línea diagonal
    for (int row = 3; row < 6; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (board[row][col] == piece && board[row-1][col+1] == piece && board[row-2][col+2] == piece && board[row-3][col+3] == piece) {
                return true; //Verifica 4 piezas consecutivas diagonales
            }
        }
    }
    //Verrifica diagonales con pendiente negativa
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (board[row][col] == piece && board[row+1][col+1] == piece && board[row+2][col+2] == piece && board[row+3][col+3] == piece) {
                return true; //Verifica 4 piezas consecutivas diagonales
            }
        }
    }
    return false;
}
