#include "Sprite.h"
#include <iostream>

Sprite::Sprite()
{
    width = 0;
    height = 0;
}

bool Sprite::LoadImage(const std::string& filepath)
{
    if (!texture.loadFromFile(filepath))
    {
        std::cerr << "Échec du chargement de la texture : " << filepath << std::endl;
        return false;
    }

    sprite.emplace(texture);

    width = static_cast<float>(texture.getSize().x);
    height = static_cast<float>(texture.getSize().y);
    return true;
}

void Sprite::Draw(sf::RenderTarget& target) const
{
    if (sprite)
    {
        target.draw(*sprite);
    }
}

void Sprite::SetPosition(float x, float y)
{
    if (sprite)
    {
        sprite->setPosition({ x, y });
    }
}

std::pair<float, float> Sprite::GetPosition() const
{
    if (sprite)
    {
        sf::Vector2f pos = sprite->getPosition();
        return { pos.x, pos.y };
    }
    return { 0, 0 };
}

void Sprite::SetSize(float w, float h)
{
    if (sprite)
    {
        width = w;
        height = h;
        sprite->setScale({ w / static_cast<float>(texture.getSize().x), h / static_cast<float>(texture.getSize().y) });
    }
}

std::pair<float, float> Sprite::GetSize() const
{
    return { width, height };
}