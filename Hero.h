#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>

class Hero : public sf::Sprite {
private:
    std::vector<sf::IntRect> idle_frames;
    std::vector<sf::IntRect> walk_frames_left;
    std::vector<sf::IntRect> walk_frames_right;
    std::vector<sf::IntRect>* current_frames;
    sf::Texture idle_texture;
    sf::Texture walk_texture_left;
    sf::Texture walk_texture_right;
    int animation_fps;
    int current_frame;
    float elapsed_time;
    float time_per_frame;
    bool is_moving;
    bool is_moving_left;

public:

    Hero(int fps);
    void setIdleTexture(const sf::Texture& texture);
    void setWalkTextureLeft(const sf::Texture& texture);
    void setWalkTextureRight(const sf::Texture& texture);
    void add_idle_frame(const sf::IntRect& rect);
    void add_walk_frame_left(const sf::IntRect& rect);
    void add_walk_frame_right(const sf::IntRect& rect);
    void setMoving(bool moving, bool movingLeft);
    void update(float delta_time);
};
