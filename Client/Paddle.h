#ifndef PADDLE_H
#define PADDLE_H

#include <SFML/Graphics.hpp>

class Paddle {
public:
    sf::RectangleShape shape;

    Paddle(float x, float y);
    void MoveUp(float deltaTime);
    void MoveDown(float deltaTime, float windowHeight);
    void Draw(sf::RenderWindow& window);
};

#endif
