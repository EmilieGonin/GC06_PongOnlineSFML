#pragma once

#include "Scene.h"
#include "SceneManager.h"
#include <string>

class WaitingScene : public Scene {
public:
    WaitingScene(const std::string& message);
    ~WaitingScene();

    void Init() override;
    void Update(float deltaTime) override;
    void Draw(sf::RenderTarget& target) override;
    void Unload() override;

private:
    std::string waitingMessage;
    sf::Font font;
    sf::Text text;
};