#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Enemy.h"
#include "Hero.h"

void loadTexture(sf::Texture& texture, const std::string& filename) {
	if (!texture.loadFromFile(filename)) {
		std::cerr << "Could not load texture from " << filename << std::endl;
		exit(1);
	}
}

void setupHero(Hero& hero, int fps) {
	sf::Texture idle_texture, walk_texture_left, walk_texture_right;
	loadTexture(idle_texture, "Woman_Idle.png");
	loadTexture(walk_texture_left, "Woman_Walk_Left.png");
	loadTexture(walk_texture_right, "Woman_Walk_Right.png");

	hero.setIdleTexture(idle_texture);
	hero.setWalkTextureLeft(walk_texture_left);
	hero.setWalkTextureRight(walk_texture_right);


	for (int i = 0; i < 5; ++i) {
		hero.add_idle_frame(sf::IntRect(128 * i, 0, 128, 128));
	}

	for (int i = 0; i < 8; ++i) {
		hero.add_walk_frame_left(sf::IntRect(128 * i, 0, 128, 128));
		hero.add_walk_frame_right(sf::IntRect(128 * i, 0, 128, 128));
	}

	hero.setPosition(50, 470);
}

void setupEnemy(Enemy& enemy, sf::Texture& texture, const std::string& textureFile, float scale, sf::Vector2f position, const std::vector<sf::IntRect>& frames) {
	loadTexture(texture, textureFile);
	enemy.setTexture(texture);
	enemy.setScale(scale, scale);
	enemy.setPosition(position);

	for (const auto& frame : frames) {
		enemy.add_animation_frame(frame);
	}
}


sf::Vector2f handleMovement(Hero& hero, float speed) {
	sf::Vector2f movement(0.f, 0.f);
	bool movingLeft = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		movement.x -= speed;
		movingLeft = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		movement.x += speed;
		movingLeft = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		movement.y -= speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		movement.y += speed;
	}

	hero.setMoving(movement.x != 0 || movement.y != 0, movingLeft);
	return movement;
}

void checkBounds(sf::Vector2f& movement, const sf::Vector2f& position, const sf::FloatRect& windowBounds, const sf::FloatRect& heroBounds) {
	if (position.x + movement.x < windowBounds.left) {
		movement.x = windowBounds.left - position.x;
	}
	if (position.x + heroBounds.width + movement.x > windowBounds.left + windowBounds.width) {
		movement.x = windowBounds.left + windowBounds.width - position.x - heroBounds.width;
	}
	if (position.y + movement.y < windowBounds.top) {
		movement.y = windowBounds.top - position.y;
	}
	if (position.y + heroBounds.height + movement.y > windowBounds.top + windowBounds.height) {
		movement.y = windowBounds.top + windowBounds.height - position.y - heroBounds.height;
	}
}



int main() {
	sf::RenderWindow window(sf::VideoMode(1100, 600), "Game project v1");
	sf::Clock clock;

	int animation_fps = 10;
	Hero hero(animation_fps);
	setupHero(hero, animation_fps);

	std::vector<sf::Texture> enemyTextures(4);
	std::vector<Enemy> enemies(4, Enemy(animation_fps));

	setupEnemy(enemies[0], enemyTextures[0], "enemy1_Idle.png", 1.5, sf::Vector2f(300, 150), {
		{0, 0, 128, 128},
		{128, 0, 128, 128},
		{256, 0, 128, 128},
		{384, 0, 128, 128},
		{512, 0, 128, 128},
		{640, 0, 128, 128},
		{768, 0, 128, 128}
		});

	setupEnemy(enemies[1], enemyTextures[1], "enemy2_Idle.png", 1.5, sf::Vector2f(500, 80), {
		{0, 0, 128, 128},
		{128, 0, 128, 128},
		{256, 0, 128, 128},
		{384, 0, 128, 128},
		{512, 0, 128, 128}
		});

	setupEnemy(enemies[2], enemyTextures[2], "enemy3_Idle.png", 2, sf::Vector2f(620, 250), {
		{0, 0, 96, 96},
		{96, 0, 96, 96},
		{192, 0, 96, 96},
		{288, 0, 96, 96},
		{384, 0, 96, 96}
		});

	setupEnemy(enemies[3], enemyTextures[3], "enemy4_Idle.png", 2, sf::Vector2f(800, 210), {
		{0, 0, 128, 128},
		{128, 0, 128, 128},
		{256, 0, 128, 128},
		{384, 0, 128, 128},
		{512, 0, 128, 128}
		});

	sf::Texture map_texture;
	loadTexture(map_texture, "mapka0.png");
	sf::Sprite map(map_texture);
	map.setScale(0.5, 0.5);

	sf::Texture map1_texture;
	loadTexture(map1_texture, "mapka1.png");
	sf::Sprite map1(map1_texture);
	map1.setScale(1, 1);

	sf::Texture questTexture;
	loadTexture(questTexture, "quest.png");
	sf::Sprite questSprite(questTexture);
	questSprite.setPosition(370, 180);
	questSprite.setScale(0.4, 0.4);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed) {

				if (event.mouseButton.button == sf::Mouse::Left) {
					sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
					if (questSprite.getGlobalBounds().contains(mousePosition)) {

						sf::RenderWindow subWindow(sf::VideoMode(950, 650), "Quest Window");
						Hero subHero(10);
						setupHero(subHero, 10);
						sf::FloatRect windowBounds(0, 0, subWindow.getSize().x, subWindow.getSize().y);

						bool stoneVisible = true;
						bool check = true;
						bool stoneVisible2 = true;
						bool check2 = true;
						bool stoneVisible3 = true;
						bool check3 = true;
						bool stoneVisible4 = true;
						bool check4 = true;
						bool stoneVisible5 = true;
						bool check5 = true;
						bool stoneVisible6 = true;
						bool check6 = true;
						bool stoneVisible7 = true;
						bool check7 = true;
						int it = 0;
						bool end1 = false;
						//stone 1
						sf::Texture questsquareTexture;
						loadTexture(questsquareTexture, "quest1_square.png");
						sf::Sprite questsquareSprite(questsquareTexture);
						questsquareSprite.setPosition(370, 250);

						sf::Texture stoneTexture;
						loadTexture(stoneTexture, "stone.png");
						sf::Sprite stoneSprite(stoneTexture);
						stoneSprite.setPosition(270, 250);
						stoneSprite.setScale(2, 2);

						//stone 2
						sf::Texture questsquare2Texture;
						loadTexture(questsquare2Texture, "quest1_square.png");
						sf::Sprite questsquare2Sprite(questsquare2Texture);
						questsquare2Sprite.setPosition(410, 250);

						sf::Texture stone2Texture;
						loadTexture(stone2Texture, "stone.png");
						sf::Sprite stone2Sprite(stone2Texture);
						stone2Sprite.setPosition(130, 150);
						stone2Sprite.setScale(2, 2);

						//stone 3
						sf::Texture questsquare3Texture;
						loadTexture(questsquare3Texture, "quest1_square.png");
						sf::Sprite questsquare3Sprite(questsquare3Texture);
						questsquare3Sprite.setPosition(450, 250);

						sf::Texture stone3Texture;
						loadTexture(stone3Texture, "stone.png");
						sf::Sprite stone3Sprite(stone3Texture);
						stone3Sprite.setPosition(330, 350);
						stone3Sprite.setScale(2, 2);

						//stone 4
						sf::Texture questsquare4Texture;
						loadTexture(questsquare4Texture, "quest1_square.png");
						sf::Sprite questsquare4Sprite(questsquare4Texture);
						questsquare4Sprite.setPosition(410, 290);

						sf::Texture stone4Texture;
						loadTexture(stone4Texture, "stone.png");
						sf::Sprite stone4Sprite(stone4Texture);
						stone4Sprite.setPosition(600, 300);
						stone4Sprite.setScale(2, 2);

						//stone 5
						sf::Texture questsquare5Texture;
						loadTexture(questsquare5Texture, "quest1_square.png");
						sf::Sprite questsquare5Sprite(questsquare5Texture);
						questsquare5Sprite.setPosition(410, 330);

						sf::Texture stone5Texture;
						loadTexture(stone5Texture, "stone.png");
						sf::Sprite stone5Sprite(stone5Texture);
						stone5Sprite.setPosition(620, 450);
						stone5Sprite.setScale(2, 2);

						//stone 6
						sf::Texture questsquare6Texture;
						loadTexture(questsquare6Texture, "quest1_square.png");
						sf::Sprite questsquare6Sprite(questsquare6Texture);
						questsquare6Sprite.setPosition(450, 330);

						sf::Texture stone6Texture;
						loadTexture(stone6Texture, "stone.png");
						sf::Sprite stone6Sprite(stone6Texture);
						stone6Sprite.setPosition(180, 400);
						stone6Sprite.setScale(2, 2);

						//stone 7
						sf::Texture questsquare7Texture;
						loadTexture(questsquare7Texture, "quest1_square.png");
						sf::Sprite questsquare7Sprite(questsquare7Texture);
						questsquare7Sprite.setPosition(370, 330);

						sf::Texture stone7Texture;
						loadTexture(stone7Texture, "stone.png");
						sf::Sprite stone7Sprite(stone7Texture);
						stone7Sprite.setPosition(300, 200);
						stone7Sprite.setScale(2, 2);

						//end window
						sf::Texture end1Texture;
						loadTexture(end1Texture, "endquest1.png");
						sf::Sprite end1Sprite(end1Texture);
						end1Sprite.setPosition(270, 130);

						while (subWindow.isOpen()) {


							sf::Event subEvent;
							while (subWindow.pollEvent(subEvent)) {
								if (subEvent.type == sf::Event::Closed) {
									subWindow.close();
								}
								//stone 1
								if (subHero.getPosition().x >= 200 && subHero.getPosition().x <= 220 && subHero.getPosition().y >= 130 && subHero.getPosition().y <= 150 && check == true) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stoneVisible = false;
										check = false;
									}
								}

								if (subHero.getPosition().x >= 300 && subHero.getPosition().x <= 340 && subHero.getPosition().y >= 130 && subHero.getPosition().y <= 160 && check == false) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stoneSprite.setPosition(370, 250);
										stoneSprite.setScale(3, 3);
										stoneVisible = true;
										it++;
										std::cout << it << std::endl;

									}
								}

								//stone 2 - 130,150
								if (subHero.getPosition().x >= 60 && subHero.getPosition().x <= 80 && subHero.getPosition().y >= 20 && subHero.getPosition().y <= 40 && check2 == true) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stoneVisible2 = false;
										check2 = false;
									}
								}

								if (subHero.getPosition().x >= 340 && subHero.getPosition().x <= 380 && subHero.getPosition().y >= 130 && subHero.getPosition().y <= 160 && check2 == false) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stone2Sprite.setPosition(410, 250);
										stone2Sprite.setScale(3, 3);
										stoneVisible2 = true;
										it++;
										std::cout << it << std::endl;
									}
								}
								//stone 3 - 330,350
								if (subHero.getPosition().x >= 260 && subHero.getPosition().x <= 280 && subHero.getPosition().y >= 220 && subHero.getPosition().y <= 240 && check3 == true) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stoneVisible3 = false;
										check3 = false;
									}
								}

								if (subHero.getPosition().x >= 380 && subHero.getPosition().x <= 420 && subHero.getPosition().y >= 130 && subHero.getPosition().y <= 160 && check3 == false) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stone3Sprite.setPosition(450, 250);
										stone3Sprite.setScale(3, 3);
										stoneVisible3 = true;
										it++;
										std::cout << it << std::endl;
									}
								}
								//stone 4 - 600,300
								if (subHero.getPosition().x >=530 && subHero.getPosition().x <= 550 && subHero.getPosition().y >= 170 && subHero.getPosition().y <= 190 && check4 == true) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stoneVisible4 = false;
										check4 = false;
									}
								}

								if (subHero.getPosition().x >= 340 && subHero.getPosition().x <= 380 && subHero.getPosition().y >= 170 && subHero.getPosition().y <= 200 && check4 == false) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stone4Sprite.setPosition(410, 290);
										stone4Sprite.setScale(3, 3);
										stoneVisible4 = true;
										it++;
										std::cout << it << std::endl;
									}
								}
								//stone 5 - 620,450
								if (subHero.getPosition().x >= 550 && subHero.getPosition().x <= 570 && subHero.getPosition().y >= 320 && subHero.getPosition().y <= 340 && check5 == true) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stoneVisible5 = false;
										check5 = false;
									}
								}

								if (subHero.getPosition().x >= 340 && subHero.getPosition().x <= 380 && subHero.getPosition().y >= 210 && subHero.getPosition().y <= 240 && check5 == false) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stone5Sprite.setPosition(410, 330);
										stone5Sprite.setScale(3, 3);
										stoneVisible5 = true;
										it++;
										std::cout << it << std::endl;
									}
								}
								//stone 6 - 180,400
								if (subHero.getPosition().x >= 110 && subHero.getPosition().x <= 130 && subHero.getPosition().y >= 270 && subHero.getPosition().y <= 290 && check6 == true) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stoneVisible6 = false;
										check6 = false;
									}
								}

								if (subHero.getPosition().x >= 380 && subHero.getPosition().x <= 420 && subHero.getPosition().y >= 210 && subHero.getPosition().y <= 240 && check6 == false) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stone6Sprite.setPosition(450, 330);
										stone6Sprite.setScale(3, 3);
										stoneVisible6 = true;
										it++;
										std::cout << it << std::endl;
									}
								}
								//stone 7 - 300,200
								if (subHero.getPosition().x >= 230 && subHero.getPosition().x <= 250 && subHero.getPosition().y >= 70 && subHero.getPosition().y <= 90 && check7 == true) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stoneVisible7 = false;
										check7 = false;
									}
								}

								if (subHero.getPosition().x >= 300 && subHero.getPosition().x <= 340 && subHero.getPosition().y >= 210 && subHero.getPosition().y <= 240 && check7 == false) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stone7Sprite.setPosition(370, 330);
										stone7Sprite.setScale(3, 3);
										stoneVisible7 = true;
										it++;
										std::cout << it << std::endl;
									}
								}

								if (it == 14) {
									end1 = true;
								}
								
							}

							float delta_time = clock.restart().asSeconds();
							float hero_speed = 0.2;

							sf::Vector2f movement = handleMovement(subHero, hero_speed);
							sf::Vector2f hero_pos = subHero.getPosition();
							sf::FloatRect hero_bounds = subHero.getGlobalBounds();

							checkBounds(movement, hero_pos, windowBounds, hero_bounds);
							subHero.move(movement);

							subHero.update(delta_time);

							subWindow.clear();
							subWindow.draw(map1);
							subWindow.draw(questsquareSprite);
							subWindow.draw(questsquare2Sprite);
							subWindow.draw(questsquare3Sprite);
							subWindow.draw(questsquare4Sprite);
							subWindow.draw(questsquare5Sprite);
							subWindow.draw(questsquare6Sprite);
							subWindow.draw(questsquare7Sprite);
							if (stoneVisible) {
								subWindow.draw(stoneSprite);
							}
							if (stoneVisible2) {
								subWindow.draw(stone2Sprite);
							}
							if (stoneVisible3) {
								subWindow.draw(stone3Sprite);
							}
							if (stoneVisible4) {
								subWindow.draw(stone4Sprite);
							}
							if (stoneVisible5) {
								subWindow.draw(stone5Sprite);
							}
							if (stoneVisible6) {
								subWindow.draw(stone6Sprite);
							}
							if (stoneVisible7) {
								subWindow.draw(stone7Sprite);
							}
							subWindow.draw(subHero);
							if (end1) {
								subWindow.draw(end1Sprite);
							}
							
							subWindow.display();
						}
					}
				}
			}
		}

		float delta_time = clock.restart().asSeconds();
		float hero_speed = 0.2;

		sf::Vector2f movement = handleMovement(hero, hero_speed);
		sf::Vector2f hero_pos = hero.getPosition();
		sf::FloatRect hero_bounds = hero.getGlobalBounds();
		sf::FloatRect window_bounds(0, 0, window.getSize().x, window.getSize().y);

		checkBounds(movement, hero_pos, window_bounds, hero_bounds);
		hero.move(movement);

		for (auto& enemy : enemies) {
			enemy.update(delta_time);
		}
		hero.update(delta_time);

		window.clear();
		window.draw(map);
		for (const auto& enemy : enemies) {
			window.draw(enemy);
		}
		window.draw(questSprite);
		window.draw(hero);
		window.display();
	}


	return 0;
}