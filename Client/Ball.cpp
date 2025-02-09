#include "Ball.h"
#include <iostream>

Ball::Ball(float startX, float startY)
{
    position.x = startX;
    position.y = startY;

    if (!spriteBall.LoadImage("ball.png"))
    {
        std::cerr << "Erreur de chargement de ball.png" << std::endl;
    }

    spriteBall.SetSize(20.f, 20.f);
    spriteBall.SetPosition(startX, startY);
    radius = 10.f;
}

void Ball::Update(float deltaTime)
{
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    if (position.y <= 0 || position.y + radius * 2 >= 600)
    {
        InvertDirectionY();
    }

    spriteBall.SetPosition(position.x, position.y);
}

void Ball::Draw(sf::RenderTarget& target) const
{
    spriteBall.Draw(target);
}

void Ball::InvertDirectionX()
{
    velocity.x = -velocity.x;
}

void Ball::InvertDirectionY()
{
    velocity.y = -velocity.y;
}

float Ball::GetX() const
{
    return position.x;
}

float Ball::GetY() const
{
    return position.y;
}

sf::Vector2f Ball::GetSize() const
{
    return { radius * 2.f, radius * 2.f };
}