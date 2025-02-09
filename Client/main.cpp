#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 54000
#define BUFFER_SIZE 1024

bool initWinsock() {
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}

SOCKET createUDPSocket() {
    return socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

void sendMessage(SOCKET clientSocket, sockaddr_in& serverAddr) {
    std::string message;
    char buffer[BUFFER_SIZE];
    int serverAddrSize = sizeof(serverAddr);

    while (true) {
        std::cout << "Entrez un message à envoyer au serveur (ou 'exit' pour quitter) : ";
        std::getline(std::cin, message);

        if (message == "exit") break;

        sendto(clientSocket, message.c_str(), message.size(), 0, (sockaddr*)&serverAddr, serverAddrSize);

        int bytesReceived = recvfrom(clientSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&serverAddr, &serverAddrSize);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Erreur recvfrom: " << WSAGetLastError() << std::endl;
            continue;
        }

        buffer[bytesReceived] = '\0';
        std::cout << "Réponse du serveur: " << buffer << std::endl;
    }
}

int main() {
    if (!initWinsock()) {
        std::cerr << "Échec de l'initialisation de Winsock." << std::endl;
        return 1;
    }

    SOCKET clientSocket = createUDPSocket();
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Erreur de création de socket." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    sendMessage(clientSocket, serverAddr);

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}