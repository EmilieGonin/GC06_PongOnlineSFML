#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Scene
{
public:
    virtual void Init() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Draw(sf::RenderTarget& target) = 0;
    virtual void Unload() = 0;
    virtual ~Scene() {}
};