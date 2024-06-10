#include "Projectile.h"

Projectile::Projectile(const sf::Texture& texture, const sf::Vector2f& startPosition, float speed)
    : speed(speed) {
    setTexture(texture);
    setScale(2.0f, 2.0f);
    sf::FloatRect bounds = getGlobalBounds();
    setPosition(startPosition.x - bounds.width / 2, startPosition.y - bounds.height / 2);
    velocity = sf::Vector2f(-speed, 0);
}

void Projectile::update(float deltaTime) {
    move(velocity * deltaTime);
}

bool Projectile::isOffScreen(const sf::FloatRect& screenBounds) const {
    return !screenBounds.intersects(getGlobalBounds());
}