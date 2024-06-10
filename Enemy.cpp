#include "Enemy.h"

void Enemy::add_animation_frame(const sf::IntRect& rect) {
    frames.push_back(rect);
}

void Enemy::setAttackTexture(const sf::Texture& texture) 
{ attackTexture = texture; 
}

void Enemy::add_attack_frame(const sf::IntRect& frame)
{
    attackFrames.push_back(frame); 
}

Enemy::Enemy(int fps) : animation_fps(fps), current_frame(0), elapsed_time(0.0f) {
    time_per_frame = 1.0f / static_cast<float>(animation_fps);
}

void Enemy::shoot()
{
    projectileVisible = true;
    projectile.setPosition(getPosition().x, getPosition().y);
}

void Enemy::update(float delta_time) {
    if (frames.empty()) return;

    elapsed_time += delta_time;

    if (elapsed_time >= time_per_frame) {
        elapsed_time -= time_per_frame;
        current_frame = (current_frame + 1) % frames.size();
        setTextureRect(frames[current_frame]);
    }
}

void Enemy::moveProjectile(float deltaTime)
{
    if (projectileVisible) {
        projectile.move(projectileSpeed * deltaTime, 0);
    }
}

const sf::Sprite& Enemy::getProjectile() const
{
    return projectile;
}

bool Enemy::isProjectileVisible() const
{
    return projectileVisible;
}

void Enemy::resetProjectile()
{
    projectileVisible = false;
    projectile.setPosition(-100, -100); 
}

