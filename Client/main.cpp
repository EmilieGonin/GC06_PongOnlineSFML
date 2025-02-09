#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <optional>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 54000
#define BUFFER_SIZE 1024

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Client UDP - Pong");

    sf::RectangleShape paddle1({ 10.f, 100.f });
    paddle1.setFillColor(sf::Color(0, 255, 0));
    paddle1.setPosition({ 50.f, 250.f });

    sf::RectangleShape paddle2({ 10.f, 100.f });
    paddle2.setFillColor(sf::Color(255, 0, 0));
    paddle2.setPosition({ 740.f, 250.f });

    sf::CircleShape ball(10.f);
    ball.setFillColor(sf::Color(255, 255, 255));
    ball.setPosition({ 400.f, 300.f });

    sf::Font font;
    if (!font.openFromFile("Roboto.ttf")) {
        std::cerr << "Erreur chargement police\n";
        return -1;
    }

    sf::Text scoreText(font);
    scoreText.setString("0 - 0");
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color(255, 255, 255));
    scoreText.setPosition({ 350.f, 10.f });

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
        return 0;
    }

    bool isPlayer1 = (role == "J1");

    while (window.isOpen()) {
        while (std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        std::string input;
        if (isPlayer1) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) input = "Z";
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) input = "S";
        }
        else {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) input = "P";
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M)) input = "M";
        }

        if (!input.empty()) {
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

                std::stringstream ss(data);
                std::string p1Y, p2Y, ballX, ballY, s1, s2;
                std::getline(ss, p1Y, ',');
                std::getline(ss, p2Y, ',');
                std::getline(ss, ballX, ',');
                std::getline(ss, ballY, ',');
                std::getline(ss, s1, ',');
                std::getline(ss, s2, ',');

                paddle1.setPosition({ 50.f, std::stof(p1Y) });
                paddle2.setPosition({ 740.f, std::stof(p2Y) });
                ball.setPosition({ std::stof(ballX), std::stof(ballY) });

                scoreText.setString(s1 + " - " + s2);
            }
        }

        window.clear();
        window.draw(paddle1);
        window.draw(paddle2);
        window.draw(ball);
        window.draw(scoreText);
        window.display();
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}