#include "Enemy.h"

void Enemy::add_animation_frame(const sf::IntRect& rect) {
    frames.push_back(rect);
}

Enemy::Enemy(int fps) : animation_fps(fps), current_frame(0), elapsed_time(0.0f) {
    time_per_frame = 1.0f / static_cast<float>(animation_fps);
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
