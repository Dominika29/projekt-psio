// Hero.cpp
#include "Hero.h"

Hero::Hero(int fps) : animation_fps(fps), current_frame(0), elapsed_time(0.0f), is_moving(false), is_moving_left(false), attacking(false) {
    time_per_frame = 1.0f / static_cast<float>(animation_fps);
    current_frames = &idle_frames;
  
   
}

void Hero::setIdleTexture(const sf::Texture& texture) {
    idle_texture = texture;
    if (!is_moving && !attacking) {
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

void Hero::setAttackTexture(const sf::Texture& texture) {
    attackTexture = texture;
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

void Hero::add_attack_frame(const sf::IntRect& rect) {
    attack_frames.push_back(rect);
}

void Hero::setMoving(bool moving, bool movingLeft) {
    if (is_moving != moving || is_moving_left != movingLeft) {
        is_moving = moving;
        is_moving_left = movingLeft;
        current_frame = 0;
        elapsed_time = 0.0f;
        if (is_moving) {
            attacking = false;
            current_frames = is_moving_left ? &walk_frames_left : &walk_frames_right;
            setTexture(is_moving_left ? walk_texture_left : walk_texture_right);
        }
        else {
            current_frames = &idle_frames;
            setTexture(idle_texture);
        }
    }
}

void Hero::setAttacking(bool isAttacking) {
    attacking = isAttacking;
    is_moving = false;
    current_frame = 0;
    elapsed_time = 0;
    if (attacking) {
        current_frames = &attack_frames;
        setTexture(attackTexture);
    }
}

void Hero::attack() {
    setAttacking(true);
}

void Hero::update(float delta_time) {
    if (current_frames->empty()) return;

    elapsed_time += delta_time;

    if (elapsed_time >= time_per_frame) {
        elapsed_time -= time_per_frame;
        current_frame = (current_frame + 1) % current_frames->size();
        setTextureRect((*current_frames)[current_frame]);
        if (attacking && current_frame == current_frames->size() - 1) {
            setAttacking(false); 
        }
    }

    hpBar.setSize(sf::Vector2f(100 * (hp / 100.0f), 10));
    hpBar.setPosition(getPosition().x, getPosition().y - 20);
}

void Hero::decreaseHP(float amount) {
    hp -= amount;
    if (hp < 0) hp = 0;
}

float Hero::getHP() const {
    return hp;
}

void Hero::drawHP(sf::RenderWindow& window) {
    window.draw(hpBar);
}
