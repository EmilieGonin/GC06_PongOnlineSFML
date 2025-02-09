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
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Client UDP - 2 Joueurs");

    sf::RectangleShape player1(sf::Vector2f(50.f, 50.f));
    player1.setFillColor(sf::Color::Green);
    player1.setPosition(sf::Vector2f(200.f, 300.f));

    sf::RectangleShape player2(sf::Vector2f(50.f, 50.f));
    player2.setFillColor(sf::Color::Red);
    player2.setPosition(sf::Vector2f(400.f, 300.f));

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in serverAddr = { AF_INET, htons(SERVER_PORT) };
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    std::cout << "Connexion au serveur..." << std::endl;
    sendto(clientSocket, "connect", 7, 0, (sockaddr*)&serverAddr, sizeof(serverAddr));

    char roleBuffer[BUFFER_SIZE];
    int serverLen = sizeof(serverAddr);
    recvfrom(clientSocket, roleBuffer, BUFFER_SIZE, 0, (sockaddr*)&serverAddr, &serverLen);
    std::string role(roleBuffer);

    role = role.substr(0, 2);
    std::cout << "Réponse du serveur : " << role << std::endl;

    if (role == "full") {
        std::cout << "Partie déjà pleine !" << std::endl;
        return 0;
    }

    bool isPlayer1 = (role == "J1");
    std::cout << "Valeur de isPlayer1 : " << isPlayer1 << std::endl;

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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) input = "UP";
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) input = "DOWN";
        }

        if (!input.empty()) {
            std::cout << "Envoi au serveur : " << input << std::endl;
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
                std::string p1Input, p2Input;
                std::getline(ss, p1Input, ',');
                std::getline(ss, p2Input, ',');

                std::cout << "Commande reçue : Player 1 -> " << p1Input << ", Player 2 -> " << p2Input << std::endl;

                if (p1Input == "Z") player1.move(sf::Vector2f(0.f, -5.f));
                if (p1Input == "S") player1.move(sf::Vector2f(0.f, 5.f));
                if (p2Input == "UP") player2.move(sf::Vector2f(0.f, -5.f));
                if (p2Input == "DOWN") player2.move(sf::Vector2f(0.f, 5.f));
            }
        }

        window.clear();
        window.draw(player1);
        window.draw(player2);
        window.display();
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}