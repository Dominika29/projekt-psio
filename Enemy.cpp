#include "Enemy.h"

void Enemy::add_animation_frame(const sf::IntRect& rect) {
    frames.push_back(rect);
}

Enemy::Enemy(int fps) : animation_fps(fps), current_frame(0), elapsed_time(0.0f),
movingUp(true), moveSpeed(0.0f), moveDistance(0.0f),
pauseTime(1.0f), elapsedPauseTime(0.0f), isPaused(false) {
    time_per_frame = 1.0f / static_cast<float>(animation_fps);
}

void Enemy::setMovement(float distance, float speed, float pause) {
    moveDistance = distance;
    moveSpeed = speed;
    pauseTime = pause;
}

void Enemy::startMovement(const sf::Vector2f& startPos) {
    startPosition = startPos;
    endPosition = sf::Vector2f(startPos.x, startPos.y - moveDistance);
    setPosition(startPos);
}

void Enemy::update(float delta_time) {
    if (frames.empty()) return;

    elapsed_time += delta_time;

    if (elapsed_time >= time_per_frame) {
        elapsed_time -= time_per_frame;
        current_frame = (current_frame + 1) % frames.size();
        setTextureRect(frames[current_frame]);
    }

    sf::Vector2f position = getPosition();

    if (isPaused) {
        elapsedPauseTime += delta_time;
        if (elapsedPauseTime >= pauseTime) {
            isPaused = false;
            elapsedPauseTime = 0.0f;
        }
        return;
    }

    if (movingUp) {
        position.y -= moveSpeed * delta_time;
        if (position.y <= endPosition.y) {
            position.y = endPosition.y;
            movingUp = false;
            isPaused = true;
        }
    }
    else {
        position.y += moveSpeed * delta_time;
        if (position.y >= startPosition.y) {
            position.y = startPosition.y;
            movingUp = true;
            isPaused = true;
        }
    }

    setPosition(position);
}