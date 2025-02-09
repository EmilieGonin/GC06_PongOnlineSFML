#pragma once

#include "Sprite.h"

class Paddle
{
public:
    Paddle(float startX, float startY);
    void Update(bool moveUp, bool moveDown, float deltaTime);
    void Draw(sf::RenderTarget& target) const;

    float GetX() const;
    float GetY() const;
    sf::Vector2f GetSize() const;

private:
    sf::Vector2f position;
    float width;
    float height;
    float speed = 200.0f;

    Sprite spritePaddle;
};