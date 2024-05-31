#include <iostream>
#include <thread>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include "game.h"
//  g++ -Wall -std=c++11 -o cliente cliente.cpp game.cpp
// g++ -Wall -std=c++11 -o servidor servidor.cpp game.cpp

void handleClient(int clientSocket) {
    Game game;
    char buffer[256];
    // bool clientTurn = rand() % 2 == 0;
    bool clientTurn = false;

    char clientPiece = 'C';
    char serverPiece = 'S';

    while (true) {
        memset(buffer, 0, 256);
        game.printBoard();
        if (clientTurn) {
            std::cout << "Esperando turno del cliente" << std::endl;
            // read(clientSocket, buffer, 255);
            if (read(clientSocket, buffer, 255) <= 0) {
                std::cerr << "Error leyendo del socket. La conexión puede haberse perdido." << std::endl;
                break;
            }

            int col = atoi(buffer);
            if (game.dropPiece(col, clientPiece)) {
                if (game.checkWin(clientPiece)) {
                    game.printBoard();
                    std::cout << "EL CLIENTE GANA!" << std::endl;
                    break;
                }
                
                clientTurn = false;
                //imprimir valor de clienTurn por consola
                std::cout << "clientTurn: " << clientTurn << std::endl;
            }
        } else {
            std::cout << "Turno del servidor..." << std::endl;
            int col;
            std::cin >> col;
            snprintf(buffer, sizeof(buffer), "%d", col);
            if (write(clientSocket, buffer, strlen(buffer)) < 0) {
                std::cerr << "Error escribiendo al socket. La conexión puede haberse perdido." << std::endl;
                break;
            }
            if (game.dropPiece(col, serverPiece)) {
                if (game.checkWin(serverPiece)) {
                    game.printBoard();
                    std::cout << "EL SERVIDOR GANA!" << std::endl;
                    break;
                }
                clientTurn = true;
                //imprimir valor de clienTurn por consola
                std::cout << "clientTurn: " << clientTurn << std::endl;
            }
        }
        if (game.isFull()) {
            game.printBoard();
            std::cout << "empate..." << std::endl;
            break;
        }
    }
    close(clientSocket);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "servidor: " << argv[0] << " <Puerto>" << std::endl;
        return 1;
    }

//creacion del socket
    int port = atoi(argv[1]);
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error abriendo el socket" << std::endl;
        return 1;
    }
//configuracion del socket
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    //enlace del socket al puerto ingresado
    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error on binding" << std::endl;
        return 1;
    }

    listen(serverSocket, 5);
    std::cout << "Servidor escuchando el puerto " << port << std::endl;

    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientLen);
        if (clientSocket < 0) {
            std::cerr << "ERROR AL ESCUCHAR" << std::endl;
            continue;
        }
        std::cout << "Nuevo cliente conectado" << std::endl;
        std::thread(handleClient, clientSocket).detach();
    }

    close(serverSocket);
    return 0;
}
