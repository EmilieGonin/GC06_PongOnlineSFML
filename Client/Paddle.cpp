#include "Paddle.h"
#include <iostream>

Paddle::Paddle(float startX, float startY)
{
    position.x = startX;
    position.y = startY;

    if (!spritePaddle.LoadImage("paddle.png"))
    {
        std::cerr << "Erreur de chargement de paddle.png" << std::endl;
    }

    spritePaddle.SetSize(20, 100);
    spritePaddle.SetPosition(startX, startY);

    width = spritePaddle.GetSize().first;
    height = spritePaddle.GetSize().second;
}

void Paddle::Update(bool moveUp, bool moveDown, float deltaTime)
{
    if (moveUp)
    {
        position.y -= speed * deltaTime;
    }
    if (moveDown)
    {
        position.y += speed * deltaTime;
    }

    if (position.y < 0)
    {
        position.y = 0;
    }
    if (position.y + height > 600)
    {
        position.y = 600 - height;
    }

    spritePaddle.SetPosition(position.x, position.y);
}

void Paddle::Draw(sf::RenderTarget& target) const
{
    spritePaddle.Draw(target);
}

float Paddle::GetX() const
{
    return position.x;
}

float Paddle::GetY() const
{
    return position.y;
}

sf::Vector2f Paddle::GetSize() const
{
    return { width, height };
}