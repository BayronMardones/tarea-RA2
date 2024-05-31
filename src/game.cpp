#include "game.h"
#include <iostream>

Game::Game() : board(6, std::vector<char>(7, ' ')) {}

void Game::printBoard() {
    for (const auto& row : board) {
        for (const auto& cell : row) {
            std::cout << (cell == ' ' ? '.' : cell) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "1 2 3 4 5 6 7" << std::endl;
}

bool Game::dropPiece(int col, char piece) {
    if (col < 1 || col > 7 || board[0][col-1] != ' ') {
        return false;
    }
    for (int i = 5; i >= 0; --i) {
        if (board[i][col-1] == ' ') {
            board[i][col-1] = piece;
            return true;
        }
    }
    return false;
}

bool Game::checkWin(char piece) {
    return checkHorizontal(piece) || checkVertical(piece) || checkDiagonal(piece);
}

bool Game::isFull() {
    for (const auto& row : board) {
        for (const auto& cell : row) {
            if (cell == ' ') {
                return false;
            }
        }
    }
    return true;
}

bool Game::checkHorizontal(char piece) {
    for (const auto& row : board) {
        for (int col = 0; col < 4; ++col) {
            if (row[col] == piece && row[col+1] == piece && row[col+2] == piece && row[col+3] == piece) {
                return true;
            }
        }
    }
    return false;
}

bool Game::checkVertical(char piece) {
    for (int col = 0; col < 7; ++col) {
        for (int row = 0; row < 3; ++row) {
            if (board[row][col] == piece && board[row+1][col] == piece && board[row+2][col] == piece && board[row+3][col] == piece) {
                return true;
            }
        }
    }
    return false;
}

bool Game::checkDiagonal(char piece) {
    // Check for diagonals with positive slope
    for (int row = 3; row < 6; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (board[row][col] == piece && board[row-1][col+1] == piece && board[row-2][col+2] == piece && board[row-3][col+3] == piece) {
                return true;
            }
        }
    }
    // Check for diagonals with negative slope
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (board[row][col] == piece && board[row+1][col+1] == piece && board[row+2][col+2] == piece && board[row+3][col+3] == piece) {
                return true;
            }
        }
    }
    return false;
}
