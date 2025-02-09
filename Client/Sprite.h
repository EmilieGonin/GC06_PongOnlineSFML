#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <optional>

class Sprite
{
public:
    Sprite();
    ~Sprite() = default;

    bool LoadImage(const std::string& filepath);
    void Draw(sf::RenderTarget& target) const;

    void SetPosition(float x, float y);
    std::pair<float, float> GetPosition() const;

    void SetSize(float width, float height);
    std::pair<float, float> GetSize() const;

private:
    sf::Texture texture;
    std::optional<sf::Sprite> sprite;
    float width, height;
};