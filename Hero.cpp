#include "Hero.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Hero::Hero(int fps) : animation_fps(fps), current_frame(0), elapsed_time(0.0f), is_moving(false), is_moving_left(false) {
    time_per_frame = 1.0f / static_cast<float>(animation_fps);
    current_frames = &idle_frames;
}

void Hero::setIdleTexture(const sf::Texture& texture) {
    idle_texture = texture;
    if (!is_moving) {
        setTexture(idle_texture);
    }
}

void Hero::setWalkTextureLeft(const sf::Texture& texture) {
    walk_texture_left = texture;
    if (is_moving && is_moving_left) {
        setTexture(walk_texture_left);
    }
}

void Hero::setWalkTextureRight(const sf::Texture& texture) {
    walk_texture_right = texture;
    if (is_moving && !is_moving_left) {
        setTexture(walk_texture_right);
    }
}

void Hero::add_idle_frame(const sf::IntRect& rect) {
    idle_frames.push_back(rect);
}

void Hero::add_walk_frame_left(const sf::IntRect& rect) {
    walk_frames_left.push_back(rect);
}

void Hero::add_walk_frame_right(const sf::IntRect& rect) {
    walk_frames_right.push_back(rect);
}

void Hero::setMoving(bool moving, bool movingLeft) {
    if (is_moving != moving || is_moving_left != movingLeft) {
        is_moving = moving;
        is_moving_left = movingLeft;
        current_frame = 0;
        elapsed_time = 0.0f;
        if (is_moving) {
            current_frames = is_moving_left ? &walk_frames_left : &walk_frames_right;
            setTexture(is_moving_left ? walk_texture_left : walk_texture_right);
        }
        else {
            current_frames = &idle_frames;
            setTexture(idle_texture);
        }
    }
}

void Hero::update(float delta_time) {
    if (current_frames->empty()) return;

    elapsed_time += delta_time;

    if (elapsed_time >= time_per_frame) {
        elapsed_time -= time_per_frame;
        current_frame = (current_frame + 1) % current_frames->size();
        setTextureRect((*current_frames)[current_frame]);
    }
}
