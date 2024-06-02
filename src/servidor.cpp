#include <iostream>
#include <thread>
#include <vector>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include "game.h"
#include <mutex>
//  g++ -Wall -std=c++11 -o cliente cliente.cpp game.cpp
// g++ -Wall -std=c++11 -o servidor servidor.cpp game.cpp
// cliente 127.0.0.1 7777
// servidor 7777

int clientCount = 0; // Variable global para contar los clientes conectados
int playerNumber = 1; // Contador global para asignar número a los jugadores
std::mutex clientCountMutex; // Mutex para proteger la variable global

void handleClient(int clientSocket, int playerNum) {
    {
        std::lock_guard<std::mutex> lock(clientCountMutex);
        clientCount++;
        std::cout << "Clientes conectados: " << clientCount << std::endl;
    }
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
                std::cout << "Jugador [IP " << playerNum << ":puerto " << playerNum << "] juega columna " << col << std::endl;
                if (game.checkWin(clientPiece)) {//Revisar si el cliente ganó
                    // game.printBoard();
                    std::cout << "EL JUGADOR [" << playerNum << "] GANA!" << std::endl;
                    break;
                }
                
                clientTurn = false; //Cambiar el turno al servidor
            }
        } else {
            std::cout << "Turno del servidor..." << std::endl; 

            // Encontrar una columna válida para jugar
            int col;
            do {
                col = rand() % 7 + 1; // Selecciona una columna al azar entre 1 y 7
            } while (!game.dropPiece(col, serverPiece));

            snprintf(buffer, sizeof(buffer), "%d", col); // Convertir el número a string correctamente //Convertir el entero a un buffer
            if (write(clientSocket, buffer, strlen(buffer)) < 0) { 
                std::cerr << "Error escribiendo al socket. La conexión puede haberse perdido." << std::endl;
                break;
            }
            std::cout << "Juego [ IP"<< playerNum << ":puerto" << playerNum << "] servidor juega columna "<< col << std::endl;
            if (game.checkWin(serverPiece)) {
                // game.printBoard();
                std::cout << "EL SERVIDOR GANA!" << std::endl;
                break;
            }
            clientTurn = true;
        }
        if (game.isFull()) {
            // game.printBoard();
            std::cout << "empate..." << std::endl;
            break;
        }
    }
    close(clientSocket);
    // Decrementar la cantidad de clientes conectados
    {
        std::lock_guard<std::mutex> lock(clientCountMutex);
        clientCount--;
        std::cout << "Cliente desconectado. Clientes conectados: " << clientCount << std::endl;
    }
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
        int playerNum;
        {
            std::lock_guard<std::mutex> lock(clientCountMutex);
            playerNum = playerNumber++;
        }

        std::cout << "Nuevo jugador [" << playerNum << "] conectado desde el puerto " << ntohs(clientAddr.sin_port) << std::endl;
        std::thread(handleClient, clientSocket, playerNum).detach();
    }

    close(serverSocket); //Cerrar el socket
    return 0;
}
