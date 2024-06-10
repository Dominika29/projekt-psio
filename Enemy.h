#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>

class Enemy : public sf::Sprite {
private:
    std::vector<sf::IntRect> frames;
    int current_frame;
    int animation_fps;
    float time_per_frame;
    float elapsed_time;

    sf::Vector2f startPosition;
    sf::Vector2f endPosition;
    bool movingUp;
    float moveSpeed;
    float moveDistance;
    float pauseTime;
    float elapsedPauseTime;
    bool isPaused;

public:
    void add_animation_frame(const sf::IntRect& rect);
    Enemy(int fps);
    void update(float delta_time);
    void setMovement(float distance, float speed, float pause);
    void startMovement(const sf::Vector2f& startPos);
};
