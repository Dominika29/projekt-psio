#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>


class Bullet {
public:
    Bullet(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& direction, float speed);
    void update(float deltaTime) ;
    void draw(sf::RenderWindow& window) const;
    sf::FloatRect getBounds() const;

private:
    sf::Sprite sprite;
    sf::Vector2f direction;
    float speed;
  
};
