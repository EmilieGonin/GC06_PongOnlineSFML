#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <unordered_map>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 54000
#define BUFFER_SIZE 1024

int main() {
    // Initialisation de Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Erreur lors de l'initialisation de Winsock !" << std::endl;
        return 1;
    }

    // Création de la socket UDP
    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in serverAddr = { AF_INET, htons(SERVER_PORT), INADDR_ANY };
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Erreur : Impossible de lier la socket !" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Serveur en attente de joueurs..." << std::endl;

    sockaddr_in clients[2];
    int numClients = 0;

    while (true) {
        std::cout << "En attente d'un message UDP..." << std::endl;

        char buffer[BUFFER_SIZE];
        sockaddr_in clientAddr;
        int clientLen = sizeof(clientAddr);
        int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&clientAddr, &clientLen);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);

            std::cout << "Message reçu : " << buffer << " de " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;

            // Enregistrement des joueurs
            if (numClients < 2) {
                clients[numClients] = clientAddr;
                numClients++;
                std::cout << "Joueur " << numClients << " connecté !" << std::endl;
                if (numClients < 2) continue;
                std::cout << "Le jeu commence !" << std::endl;
            }

            // Transmission de l’entrée aux deux joueurs
            for (int i = 0; i < 2; i++) {
                sendto(serverSocket, buffer, bytesReceived, 0, (sockaddr*)&clients[i], sizeof(clients[i]));
            }
        }
        else {
            std::cerr << "Erreur lors de la réception des données." << std::endl;
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}