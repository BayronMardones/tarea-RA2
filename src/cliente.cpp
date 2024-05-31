#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "game.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <port>" << std::endl;
        return 1;
    }

    const char* serverIp = argv[1];
    int port = atoi(argv[2]);
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    //creacion del socket
    if (clientSocket < 0) {
        std::cerr << "Error crear el socket" << std::endl;
        return 1;
    }
    //direccion del servidor
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, serverIp, &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        close(clientSocket);
        return 1;
    }
    //conexion del socket
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
                std::cerr << "conexion fallida" << std::endl;
        return 1;
    }

    Game game;
    char buffer[256];
    bool clientTurn = false;

    while (true) {
        memset(buffer, 0, 256);
        game.printBoard();
        if (clientTurn) {
            std::cout << "Your turn. Enter column (1-7): ";
            int col;
            std::cin >> col;
            snprintf(buffer, sizeof(buffer), "%d", col);
            if (write(clientSocket, buffer, strlen(buffer)) < 0) {
                std::cerr << "Servidor finalizado." << std::endl;
                break;
            }
            // write(clientSocket, buffer, strlen(buffer));
            if (game.dropPiece(col, 'C')) {
                if (game.checkWin('C')) {
                    game.printBoard();
                    std::cout << "You win!" << std::endl;
                    break;
                }
                clientTurn = false;
            }
        } else {
            std::cout << "Waiting for server's move..." << std::endl;
            //read(clientSocket, buffer, 255);
            if (read(clientSocket, buffer, 255) <= 0) {
                std::cerr << "Servidor finalizado." << std::endl;
                break;
            }
            int col = atoi(buffer);
            if (game.dropPiece(col, 'S')) {
                if (game.checkWin('S')) {
                    game.printBoard();
                    std::cout << "Server wins!" << std::endl;
                    break;
                }
                clientTurn = true;
            }
        }
        if (game.isFull()) {
            game.printBoard();
            std::cout << "It's a tie!" << std::endl;
            break;
        }
    }

    close(clientSocket);
    return 0;
}

