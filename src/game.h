#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

class Game {
public:
    Game(); //Inicializa el tablero
    void printBoard(); //Imprime el tablero
    bool dropPiece(int col, char piece); //Coloca una pieza en la columna col
    bool checkWin(char piece); //Revisa si hay un ganador
    bool isFull(); //Revisa si el tablero está lleno

private:
    std::vector<std::vector<char>> board; //Tablero de juego
    bool checkHorizontal(char piece); //Revisa si hay 4 en línea horizontal
    bool checkVertical(char piece); //Revisa si hay 4 en línea vertical
    bool checkDiagonal(char piece); //Revisa si hay 4 en línea diagonal
};

#endif // GAME_H

