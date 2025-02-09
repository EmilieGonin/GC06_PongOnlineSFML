#pragma once

#include "Scene.h"
#include <string>
#include <memory>

enum class SceneType { MENU, WAITING, GAME };

class SceneManager {
public:
    static SceneManager& GetInstance();

    void ChangeScene(SceneType type, const std::string& message = "");

    void Update(float deltaTime);
    void Draw(sf::RenderTarget& target);

    void SetUsername(const std::string& name) { username = name; }
    void SetServerIP(const std::string& ip) { serverIP = ip; }
    void SetPlayerID(int id) { playerID = id; }

    std::string GetUsername() const { return username; }
    std::string GetServerIP() const { return serverIP; }
    int GetPlayerID() const { return playerID; }

private:
    SceneManager();
    ~SceneManager();
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    std::unique_ptr<Scene> currentScene;

    std::string username = "Joueur";
    std::string serverIP = "127.0.0.1";
    int playerID = 1;
};