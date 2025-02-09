#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <windows.h>  

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 54000
#define BUFFER_SIZE 1024

std::string player1Input = "";
std::string player2Input = "";

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in serverAddr = { AF_INET, htons(SERVER_PORT), INADDR_ANY };
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    std::cout << "Serveur en attente de joueurs..." << std::endl;

    sockaddr_in player1Addr, player2Addr;
    int player1Port = 0, player2Port = 0;
    bool gameStarted = false;

    while (true) {
        Sleep(10);

        char buffer[BUFFER_SIZE];
        sockaddr_in clientAddr;
        int clientLen = sizeof(clientAddr);
        int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&clientAddr, &clientLen);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::string message(buffer);
            int senderPort = ntohs(clientAddr.sin_port);

            std::cout << "Message reçu du client (" << senderPort << ") : " << message << std::endl;

            if (message == "connect") {
                if (!gameStarted) {
                    player1Addr = clientAddr;
                    player1Port = senderPort;
                    gameStarted = true;
                    sendto(serverSocket, "J1", 2, 0, (sockaddr*)&player1Addr, sizeof(player1Addr));
                    std::cout << "Player 1 connecté." << std::endl;
                }
                else if (player2Port == 0) {
                    player2Addr = clientAddr;
                    player2Port = senderPort;
                    sendto(serverSocket, "J2", 2, 0, (sockaddr*)&player2Addr, sizeof(player2Addr));
                    std::cout << "Player 2 connecté." << std::endl;
                }
                else {
                    sendto(serverSocket, "full", 4, 0, (sockaddr*)&clientAddr, sizeof(clientAddr));
                }
            }
            else {
                if (senderPort == player1Port) {
                    player1Input = message;
                }
                else if (senderPort == player2Port) {
                    player2Input = message;
                }

                std::string combinedInputs = player1Input + "," + player2Input;
                std::cout << "Envoi aux clients : " << combinedInputs << std::endl;

                if (player1Port != 0) {
                    sendto(serverSocket, combinedInputs.c_str(), combinedInputs.size(), 0, (sockaddr*)&player1Addr, sizeof(player1Addr));
                }
                if (player2Port != 0) {
                    sendto(serverSocket, combinedInputs.c_str(), combinedInputs.size(), 0, (sockaddr*)&player2Addr, sizeof(player2Addr));
                }
            }
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}