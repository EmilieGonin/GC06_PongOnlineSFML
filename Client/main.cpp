#include <SFML/Graphics.hpp>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 54000
#define BUFFER_SIZE 1024

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Client UDP - 2 Joueurs");

    sf::RectangleShape player(sf::Vector2f(50.f, 50.f));
    player.setFillColor(sf::Color::Green);
    player.setPosition(sf::Vector2f(200.f, 300.f));

    sf::RectangleShape otherPlayer(sf::Vector2f(50.f, 50.f));
    otherPlayer.setFillColor(sf::Color::Red);
    otherPlayer.setPosition(sf::Vector2f(400.f, 300.f));

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Erreur Winsock !" << std::endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Erreur : Impossible de créer la socket !" << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    while (window.isOpen()) {
        while (std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        sf::Vector2f movement(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) movement.x -= 5.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) movement.x += 5.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) movement.y -= 5.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) movement.y += 5.f;

        player.move(movement);

        std::string positionData = std::to_string((int)player.getPosition().x) + "," +
            std::to_string((int)player.getPosition().y);
        sendto(clientSocket, positionData.c_str(), positionData.size(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));

        char buffer[BUFFER_SIZE];
        sockaddr_in senderAddr;
        int senderLen = sizeof(senderAddr);
        int bytesReceived = recvfrom(clientSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&senderAddr, &senderLen);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            float x, y;
            char comma;
            std::stringstream ss(buffer);
            if (ss >> x >> comma >> y) {
                otherPlayer.setPosition(sf::Vector2f(x, y));
            }
        }

        window.clear();
        window.draw(player);
        window.draw(otherPlayer);
        window.display();
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}