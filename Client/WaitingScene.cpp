#include "WaitingScene.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

WaitingScene::WaitingScene(const std::string& message) : waitingMessage(message), text(font) {}

WaitingScene::~WaitingScene() {}

void WaitingScene::Init()
{
    if (!font.openFromFile("Roboto.ttf")) {
        std::cerr << "Erreur chargement police\n";
    }

    text.setFont(font);
    text.setString(waitingMessage);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color(80, 80, 80));
    text.setPosition(sf::Vector2f(100.f, 300.f));
}

void WaitingScene::Update(float deltaTime)
{
    SceneManager& sceneManager = SceneManager::GetInstance();

    /*if (NetworkManager::GetInstance().ReceiveData())
    {
        sceneManager.ChangeScene(SceneType::GAME, "");
    }*/
}

void WaitingScene::Draw(sf::RenderTarget& target)
{
    target.clear(sf::Color::White);
    target.draw(text);
}

void WaitingScene::Unload() {}