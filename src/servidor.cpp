#include <iostream>
#include <thread>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include "game.h"
//  g++ -Wall -std=c++11 -o cliente cliente.cpp game.cpp
// g++ -Wall -std=c++11 -o servidor servidor.cpp game.cpp

void handleClient(int clientSocket) { //Maneja la conexión con el cliente
    Game game;
    char buffer[256];
    // bool clientTurn = rand() % 2 == 0;
    bool clientTurn = false;

    char clientPiece = 'C';
    char serverPiece = 'S';

    while (true) { //Bucle principal del juego
        memset(buffer, 0, 256); //Limpiar el buffer
        game.printBoard(); //Muestra el tablero actual
        if (clientTurn) { //Turno del cliente
            std::cout << "Esperando turno del cliente" << std::endl; 
            // read(clientSocket, buffer, 255);
            if (read(clientSocket, buffer, 255) <= 0) { //Lee el movimiento del cliente
                std::cerr << "Error leyendo del socket. La conexión puede haberse perdido." << std::endl;
                break;
            }

            int col = atoi(buffer); //Convertir el buffer a un entero
            if (game.dropPiece(col, clientPiece)) { 
                if (game.checkWin(clientPiece)) {//Revisar si el cliente ganó
                    game.printBoard();
                    std::cout << "EL CLIENTE GANA!" << std::endl;
                    break;
                }
                
                clientTurn = false; //Cambiar el turno al servidor
                //imprimir valor de clienTurn por consola
                std::cout << "clientTurn: " << clientTurn << std::endl;
            }
        } else {
            std::cout << "Turno del servidor..." << std::endl; 
            int col;
            std::cin >> col;
            snprintf(buffer, sizeof(buffer), "%d", col); //Convertir el entero a un buffer
            if (write(clientSocket, buffer, strlen(buffer)) < 0) { 
                std::cerr << "Error escribiendo al socket. La conexión puede haberse perdido." << std::endl;
                break;
            }
            if (game.dropPiece(col, serverPiece)) { 
                if (game.checkWin(serverPiece)) { //Revisar si el servidor ganó
                    game.printBoard();
                    std::cout << "EL SERVIDOR GANA!" << std::endl;
                    break;
                }
                clientTurn = true;
                //imprimir valor de clienTurn por consola
                std::cout << "clientTurn: " << clientTurn << std::endl;
            }
        }
        if (game.isFull()) { //Revisar si hay un empate
            game.printBoard();
            std::cout << "empate..." << std::endl;
            break;
        }
    }
    close(clientSocket); 
}

int main(int argc, char *argv[]) { 
    if (argc < 2) {
        std::cerr << "servidor: " << argv[0] << " <Puerto>" << std::endl; //Mensaje de error si no se ingresa el puerto
        return 1;
    }

//creacion del socket
    int port = atoi(argv[1]);
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
    if (serverSocket < 0) {
        std::cerr << "Error abriendo el socket" << std::endl; //Mensaje de error si no se puede abrir el socket
        return 1;
    }
//configuracion del socket
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr)); //Limpiar la estructura
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    //enlace del socket al puerto ingresado
    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) { //Enlazar el socket al puerto
        std::cerr << "Error on binding" << std::endl;
        return 1;
    }

    listen(serverSocket, 5); //Escuchar en el puerto
    std::cout << "Servidor escuchando el puerto " << port << std::endl;

    while (true) { //Bucle principal del servidor
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientLen); //Aceptar la conexión
        if (clientSocket < 0) {
            std::cerr << "ERROR AL ESCUCHAR" << std::endl;
            continue;
        }
        std::cout << "Nuevo cliente conectado" << std::endl; //Mensaje de nuevo cliente conectado
        std::thread(handleClient, clientSocket).detach();
    }

    close(serverSocket); //Cerrar el socket
    return 0;
}
