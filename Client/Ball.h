#ifndef BALL_H
#define BALL_H

#include <SFML/Graphics.hpp>

class Ball {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;

    Ball(float x, float y);
    void Update(float deltaTime);
    void Draw(sf::RenderWindow& window);
    void ResetPosition(float x, float y);
};

#endif
