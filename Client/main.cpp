#include <SFML/Graphics.hpp>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

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
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in serverAddr = { AF_INET, htons(SERVER_PORT) };
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    std::cout << "Connexion au serveur..." << std::endl;
    sendto(clientSocket, "connect", 7, 0, (sockaddr*)&serverAddr, sizeof(serverAddr));

    bool isPlayer1 = false, isGameStarted = false;

    while (window.isOpen()) {
        while (std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        std::cout << "Attente d'un message du serveur..." << std::endl;

        // Réception des entrées du serveur
        char buffer[BUFFER_SIZE];
        sockaddr_in senderAddr;
        int senderLen = sizeof(senderAddr);
        int bytesReceived = recvfrom(clientSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&senderAddr, &senderLen);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << "Commande reçue du serveur : " << buffer << std::endl;

            if (strcmp(buffer, "Z") == 0 || strcmp(buffer, "UP") == 0)
                player.move(sf::Vector2f(0.f, -5.f));

            if (strcmp(buffer, "S") == 0 || strcmp(buffer, "DOWN") == 0)
                player.move(sf::Vector2f(0.f, 5.f));
        }

        // Gestion des entrées spécifiques à chaque joueur
        std::string input;
        if (isPlayer1) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) input = "Z";
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) input = "S";
        }
        else {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) input = "UP";
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) input = "DOWN";
        }

        if (!input.empty()) {
            std::cout << "Envoi au serveur : " << input << std::endl;
            sendto(clientSocket, input.c_str(), input.size(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
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