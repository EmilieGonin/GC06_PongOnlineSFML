#include <SFML/Network.hpp>
#include <iostream>
#include <unordered_map>
#include <optional>

// Ajout du hash pour `sf::IpAddress`
namespace std {
    template<>
    struct hash<sf::IpAddress> {
        std::size_t operator()(const sf::IpAddress& ip) const {
            return hash<std::string>()(ip.toString());
        }
    };
}

#define SERVER_PORT 54000

int main() {
    sf::UdpSocket socket;
    if (socket.bind(SERVER_PORT) != sf::Socket::Status::Done) {
        std::cerr << "Erreur : Impossible de lier la socket au port " << SERVER_PORT << std::endl;
        return 1;
    }

    std::cout << "Serveur en attente de messages..." << std::endl;

    std::unordered_map<sf::IpAddress, unsigned short> clients;

    while (true) {
        char buffer[1024];
        std::size_t received;
        std::optional<sf::IpAddress> senderIP;
        unsigned short senderPort;

        if (socket.receive(buffer, sizeof(buffer), received, senderIP, senderPort) == sf::Socket::Status::Done) {
            buffer[received] = '\0';
            std::string data(buffer);

            if (senderIP) { // Vérifier que l'adresse est valide
                std::cout << "Reçu de " << *senderIP << " : " << data << std::endl;

                // Enregistrer le client
                if (clients.find(*senderIP) == clients.end()) {
                    clients[*senderIP] = senderPort;
                    std::cout << "Nouveau joueur connecté : " << *senderIP << ":" << senderPort << std::endl;
                }

                // Diffuser la position aux autres joueurs
                for (const auto& client : clients) {
                    if (client.first != *senderIP) {
                        socket.send(data.c_str(), data.size(), client.first, client.second);
                    }
                }
            }
        }
    }

    return 0;
}