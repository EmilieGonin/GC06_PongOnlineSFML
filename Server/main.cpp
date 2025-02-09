#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024

bool initWinsock() {
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

SOCKET createUDPSocket() {
    return socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

bool configureServer(SOCKET& serverSocket, unsigned short port) {
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;  // Accepte toutes les connexions

    return bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) != SOCKET_ERROR;
}

void runServer(SOCKET serverSocket) {
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    std::cout << "Serveur en attente de messages..." << std::endl;

    while (true) {
        int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&clientAddr, &clientAddrSize);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Erreur recvfrom: " << WSAGetLastError() << std::endl;
            continue;
        }

        buffer[bytesReceived] = '\0';  // Assurer que la chaîne est bien terminée
        std::cout << "Message reçu: " << buffer << std::endl;

        // Répondre au client
        std::string response = "Message bien reçu !";
        sendto(serverSocket, response.c_str(), response.size(), 0, (sockaddr*)&clientAddr, clientAddrSize);
    }
}

int main() {
    if (!initWinsock()) {
        std::cerr << "Échec de l'initialisation de Winsock." << std::endl;
        return 1;
    }

    SOCKET serverSocket = createUDPSocket();
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Erreur de création de socket." << std::endl;
        WSACleanup();
        return 1;
    }

    if (!configureServer(serverSocket, 54000)) {
        std::cerr << "Erreur de configuration du serveur." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    runServer(serverSocket);

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}