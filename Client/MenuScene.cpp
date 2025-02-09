#include "MenuScene.h"
#include <iostream>
#include <SFML/Graphics.hpp>

MenuScene::MenuScene()
    : username("Joueur"), serverIP("127.0.0.1"), editUser(false), editIP(false),
    font()
{
}

MenuScene::~MenuScene()
{
}

void MenuScene::Init() {
    if (!font.loadFromFile("path_to_font.ttf")) {
        std::cerr << "Erreur de chargement de la police!" << std::endl;
    }

    titleText.setFont(font);
    titleText.setString("Menu Principal");
    titleText.setCharacterSize(30);
    titleText.setFillColor(sf::Color::Black);
    titleText.setPosition(sf::Vector2f(100.f, 50.f));

    usernameLabel.setFont(font);
    usernameLabel.setString("Pseudo:");
    usernameLabel.setCharacterSize(20);
    usernameLabel.setFillColor(sf::Color::Black);
    usernameLabel.setPosition(sf::Vector2f(100.f, 120.f));

    serverIPLabel.setFont(font);
    serverIPLabel.setString("IP Serveur:");
    serverIPLabel.setCharacterSize(20);
    serverIPLabel.setFillColor(sf::Color::Black);
    serverIPLabel.setPosition(sf::Vector2f(100.f, 170.f));

    usernameBox.setSize(sf::Vector2f(200.f, 30.f));
    usernameBox.setPosition(sf::Vector2f(300.f, 115.f));
    usernameBox.setFillColor(sf::Color(220, 220, 220));

    serverIPBox.setSize(sf::Vector2f(200.f, 30.f));
    serverIPBox.setPosition(sf::Vector2f(300.f, 165.f));
    serverIPBox.setFillColor(sf::Color(220, 220, 220));

    createGameButton.setSize(sf::Vector2f(200.f, 60.f));
    createGameButton.setPosition(sf::Vector2f(100.f, 450.f));
    createGameButton.setFillColor(sf::Color::Green);

    createGameText.setFont(font);
    createGameText.setString("Créer une partie");
    createGameText.setCharacterSize(20);
    createGameText.setFillColor(sf::Color::White);
    createGameText.setPosition(sf::Vector2f(120.f, 470.f));

    joinGameButton.setSize(sf::Vector2f(200.f, 60.f));
    joinGameButton.setPosition(sf::Vector2f(100.f, 520.f));
    joinGameButton.setFillColor(sf::Color::Blue);

    joinGameText.setFont(font);
    joinGameText.setString("Rejoindre une partie");
    joinGameText.setCharacterSize(20);
    joinGameText.setFillColor(sf::Color::White);
    joinGameText.setPosition(sf::Vector2f(110.f, 540.f));
}

void MenuScene::Update(float deltaTime) {
    SceneManager& sceneManager = SceneManager::GetInstance();

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition();

        if (createGameButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            sceneManager.SetUsername(username);
            sceneManager.SetServerIP(serverIP);
            sceneManager.SetPlayerID(1);
        }

        if (joinGameButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            sceneManager.SetUsername(username);
            sceneManager.SetServerIP(serverIP);
            sceneManager.SetPlayerID(2);
        }
    }
}

void MenuScene::Draw(sf::RenderTarget& target) {
    target.clear(sf::Color::White);

    target.draw(titleText);
    target.draw(usernameLabel);
    target.draw(serverIPLabel);

    target.draw(usernameBox);
    target.draw(serverIPBox);

    target.draw(createGameButton);
    target.draw(createGameText);

    target.draw(joinGameButton);
    target.draw(joinGameText);
}

void MenuScene::Unload() {}

void MenuScene::HandleInput(sf::Event event) {
    if (event.type == sf::Event::TextEntered) {
        if (editUser) {
            if (event.text.unicode == '\b' && !username.empty()) {
                username.pop_back();
            }
            else if (event.text.unicode >= 32 && event.text.unicode <= 126 && username.size() < 19) {
                username += static_cast<char>(event.text.unicode);
            }
        }

        if (editIP) {
            if (event.text.unicode == '\b' && !serverIP.empty()) {
                serverIP.pop_back();
            }
            else if ((isdigit(event.text.unicode) || event.text.unicode == '.') && serverIP.size() < 19) {
                serverIP += static_cast<char>(event.text.unicode);
            }
        }
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition();

        if (usernameBox.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            editUser = true;
            editIP = false;
        }

        if (serverIPBox.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            editIP = true;
            editUser = false;
        }
    }
}