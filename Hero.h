// Hero.h
#ifndef HERO_H
#define HERO_H

#include <SFML/Graphics.hpp>
#include <vector>

class Hero : public sf::Sprite {
public:
	Hero(int fps);
	void setIdleTexture(const sf::Texture& texture);
	void setWalkTextureLeft(const sf::Texture& texture);
	void setWalkTextureRight(const sf::Texture& texture);
	void setAttackTexture(const sf::Texture& texture);
	void add_idle_frame(const sf::IntRect& rect);
	void add_walk_frame_left(const sf::IntRect& rect);
	void add_walk_frame_right(const sf::IntRect& rect);
	void add_attack_frame(const sf::IntRect& rect);
	void setMoving(bool moving, bool movingLeft);
	void update(float delta_time);
	void setAttacking(bool isAttacking);
	void attack();
	void decreaseHealth(int amount);
	int getHealth() const;

private:
	sf::Texture idle_texture;
	sf::Texture walk_texture_left;
	sf::Texture walk_texture_right;
	sf::Texture attackTexture;
	std::vector<sf::IntRect> idle_frames;
	std::vector<sf::IntRect> walk_frames_left;
	std::vector<sf::IntRect> walk_frames_right;
	std::vector<sf::IntRect> attack_frames;
	std::vector<sf::IntRect> *current_frames;
	int animation_fps;
	int current_frame;
	float time_per_frame;
	float elapsed_time;
	bool is_moving;
	bool is_moving_left;
	bool attacking;
	int health = 100;
};

#endif // HERO_H