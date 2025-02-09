#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <optional>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 54000

int main() {
    // --- Initialisation de SFML ---
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Client UDP - SFML 3");

    // Création du joueur (rectangle vert)
    sf::RectangleShape player(sf::Vector2f(50.f, 50.f));
    player.setFillColor(sf::Color::Green);
    player.setPosition(sf::Vector2f(375.f, 275.f));

    // --- Configuration de la connexion UDP ---
    sf::UdpSocket socket;
    socket.bind(sf::Socket::AnyPort);

    // Vérification de l'adresse IP du serveur
    std::optional<sf::IpAddress> resolvedIP = sf::IpAddress::resolve(SERVER_IP);
    if (!resolvedIP) {
        std::cerr << "Erreur : Impossible de résoudre l'adresse IP du serveur !" << std::endl;
        return 1;
    }
    sf::IpAddress serverIP = *resolvedIP;  // Extraction de l'IP
    unsigned short serverPort = SERVER_PORT;

    // --- Boucle principale ---
    while (window.isOpen()) {
        // Gestion des événements
        while (std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // --- Gestion des déplacements ---
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

        // --- Affichage ---
        window.clear();
        window.draw(player);
        window.display();
    }

    return 0;
}