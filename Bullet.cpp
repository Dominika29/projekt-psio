#include "Bullet.h"



Bullet::Bullet(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& direction, float speed)
    : direction(direction), speed(speed)
{
    sprite.setTexture(texture);
    sprite.setPosition(position);
    sprite.setScale(3, 3);
}

void Bullet::update(float deltaTime)
{
    sprite.move(direction * speed * deltaTime);
}

void Bullet::draw(sf::RenderWindow& window) const
{
    window.draw(sprite);
}

sf::FloatRect Bullet::getBounds() const
{
    return sprite.getGlobalBounds();
}


