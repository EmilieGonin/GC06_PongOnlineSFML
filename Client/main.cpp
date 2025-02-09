#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <optional>
#include "Ball.h"
#include "Paddle.h"

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 54000
#define BUFFER_SIZE 1024

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Pong Client");

    Paddle player1(50.f, 250.f);
    Paddle player2(740.f, 250.f);
    Ball ball(400.f, 300.f);

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in serverAddr = { AF_INET, htons(SERVER_PORT) };
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    sendto(clientSocket, "connect", 7, 0, (sockaddr*)&serverAddr, sizeof(serverAddr));

    char roleBuffer[BUFFER_SIZE];
    int serverLen = sizeof(serverAddr);
    recvfrom(clientSocket, roleBuffer, BUFFER_SIZE, 0, (sockaddr*)&serverAddr, &serverLen);
    std::string role(roleBuffer);
    role = role.substr(0, 2);

    if (role == "full") {
        std::cout << "Serveur plein. Fermeture du client." << std::endl;
        return 0;
    }

    bool isPlayer1 = (role == "J1");

    while (window.isOpen()) {
        while (std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        std::string input = "0";
        if (isPlayer1) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) input = "Z";
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) input = "S";
        }
        else {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) input = "P";
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M)) input = "M";
        }

        if (input != "0") {
            sendto(clientSocket, input.c_str(), input.size(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
        }

        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(clientSocket, &readSet);

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 50000;

        int activity = select(clientSocket + 1, &readSet, NULL, NULL, &timeout);
        if (activity > 0 && FD_ISSET(clientSocket, &readSet)) {
            char buffer[BUFFER_SIZE];
            int bytesReceived = recvfrom(clientSocket, buffer, BUFFER_SIZE, 0, NULL, NULL);

            if (bytesReceived > 0) {
                buffer[bytesReceived] = '\0';
                std::string data(buffer);

                float p1Y, p2Y, ballX, ballY;
                sscanf_s(data.c_str(), "%f,%f,%f,%f", &p1Y, &p2Y, &ballX, &ballY);

                player1.shape.setPosition({ player1.shape.getPosition().x, p1Y });
                player2.shape.setPosition({ player2.shape.getPosition().x, p2Y });
                ball.shape.setPosition({ ballX, ballY });
            }
        }

        window.clear();
        window.draw(player1.shape);
        window.draw(player2.shape);
        window.draw(ball.shape);
        window.display();
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}