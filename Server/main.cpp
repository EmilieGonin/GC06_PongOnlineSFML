#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <windows.h>  // Pour Sleep()

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 54000
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in serverAddr = { AF_INET, htons(SERVER_PORT), INADDR_ANY };
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    std::cout << "Serveur en attente de joueurs..." << std::endl;

    std::unordered_map<std::string, sockaddr_in> clients;

    while (true) {
        Sleep(10);  // Réduit l'utilisation CPU

        char buffer[BUFFER_SIZE];
        sockaddr_in clientAddr;
        int clientLen = sizeof(clientAddr);
        int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&clientAddr, &clientLen);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';

            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
            std::string clientKey = std::string(clientIP) + ":" + std::to_string(ntohs(clientAddr.sin_port));

            std::cout << "Message reçu : " << buffer << " de " << clientKey << std::endl;

            // Enregistrer le client s'il n'est pas encore dans la liste
            if (clients.find(clientKey) == clients.end()) {
                clients[clientKey] = clientAddr;
                std::cout << "Nouveau joueur connecté : " << clientKey << std::endl;
            }

            // Vérifier si l'entrée est valide (Z, S, UP, DOWN)
            if (strcmp(buffer, "Z") == 0 || strcmp(buffer, "S") == 0 ||
                strcmp(buffer, "UP") == 0 || strcmp(buffer, "DOWN") == 0) {

                std::cout << "Transmet l'entrée '" << buffer << "' aux autres joueurs..." << std::endl;

                // Transmettre à tous les clients sauf l'expéditeur
                for (const auto& client : clients) {
                    if (client.first != clientKey) { // Ne pas envoyer à l'expéditeur
                        sendto(serverSocket, buffer, bytesReceived, 0, (sockaddr*)&client.second, sizeof(client.second));
                        std::cout << "Envoyé à : " << client.first << std::endl;
                    }
                }
            }
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}