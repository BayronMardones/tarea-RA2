#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

class Game {
public:
    Game();
    void printBoard();
    bool dropPiece(int col, char piece);
    bool checkWin(char piece);
    bool isFull();

private:
    std::vector<std::vector<char>> board;
    bool checkHorizontal(char piece);
    bool checkVertical(char piece);
    bool checkDiagonal(char piece);
};

#endif
