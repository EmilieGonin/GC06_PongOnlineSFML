#include "Paddle.h"

Paddle::Paddle(float x, float y) {
    shape.setSize(sf::Vector2f(10.f, 100.f));
    shape.setFillColor(sf::Color::White);
    shape.setPosition({ x, y });
}

void Paddle::MoveUp(float deltaTime) {
    if (shape.getPosition().y > 0) {
        shape.move({ 0, -200.f * deltaTime });
    }
}

void Paddle::MoveDown(float deltaTime, float windowHeight) {
    if (shape.getPosition().y + shape.getSize().y < windowHeight) {
        shape.move({ 0, 200.f * deltaTime });
    }
}

void Paddle::Draw(sf::RenderWindow& window) {
    window.draw(shape);
}