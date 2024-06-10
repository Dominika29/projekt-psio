#pragma once
#include<SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>

class Enemy : public sf::Sprite 
{
private:
	std::vector<sf::IntRect> frames;
	std::vector<sf::IntRect> attackFrames; 
	int current_frame;
	int animation_fps;
	float time_per_frame;
	sf::Texture attackTexture; 
	float elapsed_time; 
	bool attacking;
	sf::Sprite projectile;
	bool projectileVisible;
	float projectileSpeed;
	float hp;


public:
	void add_animation_frame(const sf::IntRect& rect); 
	void setAttackTexture(const sf::Texture& texture); 
	void add_attack_frame(const sf::IntRect& frame);
	Enemy (int fps);
	void shoot();
	void update(float delta_time); 
	void moveProjectile(float deltaTime);
	const sf::Sprite& getProjectile() const;
	bool isProjectileVisible() const;
	void resetProjectile();
	
};

