#include "Ball.h"

Ball::Ball(float x, float y) {
    shape.setRadius(10.f);
    shape.setFillColor(sf::Color::White);
    shape.setPosition({ x, y });
    velocity = sf::Vector2f(5.f, 5.f);
}

void Ball::Update(float deltaTime) {
    shape.move(velocity * deltaTime);
}

void Ball::Draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Ball::ResetPosition(float x, float y) {
    shape.setPosition({ x, y });
}