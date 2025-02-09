#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <windows.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 54000
#define BUFFER_SIZE 1024

std::string player1Inputs = "";
std::string player2Inputs = "";
bool player1Updated = false;
bool player2Updated = false;

// Position et vitesse de la balle
float ballX = 400.f, ballY = 300.f;
float ballSpeedX = 5.f, ballSpeedY = 5.f;

// Positions des joueurs
float player1Y = 250.f;
float player2Y = 250.f;
const float playerSpeed = 10.f;
const float windowHeight = 600.f;

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
                    player1Inputs = message;
                    player1Updated = true;
                }
                else if (senderPort == player2Port) {
                    player2Inputs = message;
                    player2Updated = true;
                }

                // Mise à jour des positions des joueurs
                if (player1Inputs == "Z" && player1Y > 0) player1Y -= playerSpeed;
                if (player1Inputs == "S" && player1Y < windowHeight - 100) player1Y += playerSpeed;
                if (player2Inputs == "P" && player2Y > 0) player2Y -= playerSpeed;
                if (player2Inputs == "M" && player2Y < windowHeight - 100) player2Y += playerSpeed;

                // Mise à jour de la balle
                ballX += ballSpeedX;
                ballY += ballSpeedY;

                // Rebond sur le haut et le bas
                if (ballY <= 0 || ballY >= windowHeight - 10) {
                    ballSpeedY *= -1;
                }

                // Vérification des collisions avec les joueurs
                if (ballX <= 60 && ballY >= player1Y && ballY <= player1Y + 100) {
                    ballSpeedX *= -1;
                    ballX = 60; // Évite que la balle se colle à la raquette
                }
                if (ballX >= 730 && ballY >= player2Y && ballY <= player2Y + 100) {
                    ballSpeedX *= -1;
                    ballX = 730;
                }

                // Si la balle sort, on la remet au centre
                if (ballX < 0 || ballX > 800) {
                    ballX = 400;
                    ballY = 300;
                    ballSpeedX *= -1;
                }

                // Création du message de mise à jour
                std::string updateMessage = std::to_string(player1Y) + "," +
                    std::to_string(player2Y) + "," +
                    std::to_string(ballX) + "," +
                    std::to_string(ballY);

                std::cout << "Envoi aux clients : " << updateMessage << std::endl;

                // Envoi aux clients
                if (player1Port != 0) {
                    sendto(serverSocket, updateMessage.c_str(), updateMessage.size(), 0, (sockaddr*)&player1Addr, sizeof(player1Addr));
                }
                if (player2Port != 0) {
                    sendto(serverSocket, updateMessage.c_str(), updateMessage.size(), 0, (sockaddr*)&player2Addr, sizeof(player2Addr));
                }

                player1Inputs = "";
                player2Inputs = "";
                player1Updated = false;
                player2Updated = false;
            }
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}