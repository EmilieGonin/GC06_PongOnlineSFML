#pragma once

#include "Scene.h"
#include "SceneManager.h"
#include <string>

class MenuScene : public Scene {
private:
    std::string username;
    std::string serverIP;
    bool editUser;
    bool editIP;

    sf::Font font;
    sf::Text titleText;
    sf::Text usernameLabel;
    sf::Text serverIPLabel;
    sf::Text createGameText;
    sf::Text joinGameText;
    sf::RectangleShape createGameButton;
    sf::RectangleShape joinGameButton;
    sf::RectangleShape usernameBox;
    sf::RectangleShape serverIPBox;

public:
    MenuScene();
    ~MenuScene();

    void Init() override;
    void Update(float deltaTime) override;
    void Draw(sf::RenderTarget& target) override;
    void Unload() override;
    void HandleInput(sf::Event event);
};