#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <sstream>
#include <optional>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 54000

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Client UDP - 2 Joueurs");

    // Joueur local (vert)
    sf::RectangleShape player(sf::Vector2f(50.f, 50.f));
    player.setFillColor(sf::Color::Green);
    player.setPosition(sf::Vector2f(200.f, 300.f));

    // Joueur distant (rouge)
    sf::RectangleShape otherPlayer(sf::Vector2f(50.f, 50.f));
    otherPlayer.setFillColor(sf::Color::Red);
    otherPlayer.setPosition(sf::Vector2f(400.f, 300.f));

    // Configuration UDP
    sf::UdpSocket socket;
    socket.bind(sf::Socket::AnyPort);

    std::optional<sf::IpAddress> resolvedIP = sf::IpAddress::resolve(SERVER_IP);
    if (!resolvedIP) {
        std::cerr << "Erreur : Impossible de résoudre l'adresse IP du serveur !" << std::endl;
        return 1;
    }
    sf::IpAddress serverIP = *resolvedIP;
    unsigned short serverPort = SERVER_PORT;

    while (window.isOpen()) {
        while (std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // --- Déplacement du joueur local ---
        sf::Vector2f movement(0.f, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))  movement.x -= 5.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))  movement.x += 5.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))  movement.y -= 5.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))  movement.y += 5.f;

        player.move(movement);

        // --- Envoi de la position au serveur ---
        std::string positionData = std::to_string((int)player.getPosition().x) + "," +
            std::to_string((int)player.getPosition().y);
        socket.send(positionData.c_str(), positionData.size(), serverIP, serverPort);

        // --- Réception de la position du joueur distant ---
        char buffer[1024];
        std::size_t received;
        std::optional<sf::IpAddress> senderIP;
        unsigned short senderPort;
        if (socket.receive(buffer, sizeof(buffer), received, senderIP, senderPort) == sf::Socket::Status::Done) {
            buffer[received] = '\0';
            std::string data(buffer);

            float x, y;
            char comma;
            std::stringstream ss(data);
            if (ss >> x >> comma >> y) {
                otherPlayer.setPosition(sf::Vector2f(x, y));
            }
        }

        // --- Affichage ---
        window.clear();
        window.draw(player);
        window.draw(otherPlayer);
        window.display();
    }

    return 0;
}