#pragma once
#include <SFML/Graphics.hpp>

class Projectile : public sf::Sprite {
private:
    sf::Vector2f velocity;
    float speed;

public:
    Projectile(const sf::Texture& texture, const sf::Vector2f& startPosition, float speed);
    void update(float deltaTime);
    bool isOffScreen(const sf::FloatRect& screenBounds) const;
};