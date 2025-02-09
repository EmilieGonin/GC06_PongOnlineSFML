#include "SceneManager.h"
#include "MenuScene.h"
#include "WaitingScene.h"

SceneManager::SceneManager()
{
    currentScene = std::make_unique<MenuScene>();
    currentScene->Init();
}

SceneManager::~SceneManager() = default;

SceneManager& SceneManager::GetInstance()
{
    static SceneManager instance;
    return instance;
}

void SceneManager::ChangeScene(SceneType type, const std::string& message)
{
    if (currentScene)
    {
        currentScene->Unload();
        currentScene.reset();
    }

    switch (type)
    {
    case SceneType::MENU:
        currentScene = std::make_unique<MenuScene>();
        break;
    case SceneType::WAITING:
        currentScene = std::make_unique<WaitingScene>(message);
        break;
    case SceneType::GAME:
        //currentScene = std::make_unique<GameScene>();
        break;
    }

    if (currentScene)
    {
        currentScene->Init();
    }
}

void SceneManager::Update(float deltaTime)
{
    if (currentScene)
    {
        currentScene->Update(deltaTime);
    }
}

void SceneManager::Draw(sf::RenderTarget& target)
{
    if (currentScene)
    {
        currentScene->Draw(target);
    }
}