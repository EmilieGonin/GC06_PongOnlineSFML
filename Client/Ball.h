#pragma once

#include "Sprite.h"
#include <SFML/Graphics.hpp>

class Ball {
public:
    Ball(float startX, float startY);
    void Update(float deltaTime);
    void Draw(sf::RenderTarget& target) const;

    void InvertDirectionX();
    void InvertDirectionY();

    float GetX() const;
    float GetY() const;
    sf::Vector2f GetSize() const;

private:
    sf::Vector2f position;
    sf::Vector2f velocity = { 200.f, 200.f };
    float radius;

    Sprite spriteBall;
};