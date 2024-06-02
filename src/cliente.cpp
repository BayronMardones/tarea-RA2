#include <iostream>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "game.h"

int main(int argc, char *argv[]) { //Función principal
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <server_ip> <port>" << std::endl; //Mensaje de error
        return 1; 
    }

    const char* serverIp = argv[1]; //Dirección IP del servidor
    int port = atoi(argv[2]);
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0); //Creacion del socket TCP
    //Verificar si el socket se creo correctamente
    if (clientSocket < 0) {
        std::cerr << "Error crear el socket" << std::endl;
        return 1;
    }
    //Configuracion del socket
    sockaddr_in serverAddr; //Direccion del servidor
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_port = htons(port); 
    if (inet_pton(AF_INET, serverIp, &serverAddr.sin_addr) <= 0) { //Conversion de la direccion IP
        std::cerr << "Direccion no valida/ Direccion no compatible" << std::endl; //Mensaje de error
        close(clientSocket);
        return 1;
    }
    //Conexion al servidor
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) { 
                std::cerr << "Conexion fallida" << std::endl; //Mensaje de error
        return 1;
    }

    Game game; //Creacion del objeto Game
    char buffer[256]; 
    bool clientTurn = false; //Variable para saber si es el turno del cliente

    while (true) { 
        memset(buffer, 0, 256); //Limpiar el buffer
        game.printBoard(); //Muestra el tablero
        if (clientTurn) {
            std::cout << "Tu turno. Ingresa columna (1-7): "; 
            int col;
            std::cin >> col;
            snprintf(buffer, sizeof(buffer), "%d", col);
            if (write(clientSocket, buffer, strlen(buffer)) < 0) { //Escribir en el socket
                std::cerr << "Servidor finalizado." << std::endl;
                break;
            }
            // write(clientSocket, buffer, strlen(buffer));
            if (game.dropPiece(col, 'C')) {
                if (game.checkWin('C')) {
                    game.printBoard();
                    std::cout << "¡TU GANAS!" << std::endl;
                    break;
                }
                clientTurn = false;
            }
        } else {
            std::cout << "Esperando el turno del servidor..." << std::endl;
            //read(clientSocket, buffer, 255);
            if (read(clientSocket, buffer, 255) <= 0) {
                std::cerr << "Servidor finalizado." << std::endl;
                break;
            }
            int col = atoi(buffer); //Convertir el buffer a un entero
            if (game.dropPiece(col, 'S')) {
                if (game.checkWin('S')) {
                    game.printBoard();
                    std::cout << "¡EL SERVIDOR GANA!" << std::endl;
                    break;
                }
                clientTurn = true; //Cambiar el turno al cliente
            }
        }
        if (game.isFull()) {
            game.printBoard();
            std::cout << "¡Es un empate!" << std::endl;
            break;
        }
    }

    close(clientSocket); //Cierra el socket del cliente
    return 0;
}

