#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <unordered_map>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 54000
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Erreur lors de l'initialisation de Winsock !" << std::endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Erreur : Impossible de créer la socket !" << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Erreur : Impossible de lier la socket !" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Serveur en attente de messages..." << std::endl;

    std::unordered_map<std::string, sockaddr_in> clients;

    while (true) {
        char buffer[BUFFER_SIZE];
        sockaddr_in clientAddr;
        int clientLen = sizeof(clientAddr);
        int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&clientAddr, &clientLen);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::string data(buffer);

            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
            std::string clientKey = std::string(clientIP) + ":" + std::to_string(ntohs(clientAddr.sin_port));

            if (clients.find(clientKey) == clients.end()) {
                clients[clientKey] = clientAddr;
                std::cout << "Nouveau joueur connecté : " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;
            }

            for (const auto& client : clients) {
                if (client.second.sin_addr.s_addr != clientAddr.sin_addr.s_addr ||
                    client.second.sin_port != clientAddr.sin_port) {
                    sendto(serverSocket, data.c_str(), data.size(), 0, (sockaddr*)&client.second, sizeof(client.second));
                }
            }
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}