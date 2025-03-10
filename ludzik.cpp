#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random> 
#include "Enemy.h"
#include "Character.h"
#include "Hero.h"
#include "Bullet.h"
#include "Projectile.h"


// functions 

void loadTexture(sf::Texture& texture, const std::string& filename) {
	if (!texture.loadFromFile(filename)) {
		std::cerr << "Could not load texture from " << filename << std::endl;
		exit(1);
	}
}

void setupHero(Hero& hero, int fps) {
	sf::Texture attack_texture, idle_texture, walk_texture_left, walk_texture_right;
	loadTexture(idle_texture, "Woman_Idle.png");
	loadTexture(walk_texture_left, "Woman_Walk_Left.png");
	loadTexture(walk_texture_right, "Woman_Walk_Right.png");
	loadTexture(attack_texture, "Attack_3.png");

	hero.setIdleTexture(idle_texture);
	hero.setWalkTextureLeft(walk_texture_left);
	hero.setWalkTextureRight(walk_texture_right);
	hero.setAttackTexture(attack_texture);

	for (int i = 0; i < 5; ++i) {
		hero.add_idle_frame(sf::IntRect(128 * i, 55, 128, 73));
	}

	for (int i = 0; i < 8; ++i) {
		hero.add_walk_frame_left(sf::IntRect(128 * i, 55, 128, 73));
		hero.add_walk_frame_right(sf::IntRect(128 * i, 55, 128, 73));
	}
	for (int i = 0; i < 6; ++i) {
		hero.add_attack_frame(sf::IntRect(128 * i, 55, 128, 73));
	}

	hero.setPosition(250, 470);
}

void setupEnemy(Enemy& enemy, sf::Texture& idletexture, const std::string& textureFile, float scale, sf::Vector2f position, const std::vector<sf::IntRect>& frames) {
	loadTexture(idletexture, textureFile);
	enemy.setTexture(idletexture);
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

sf::Vector2f getRandomPosition(float x, float y)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distr(x, y);

	float random_number1 = distr(gen);
	float random_number2 = distr(gen);

	return sf::Vector2f(random_number1, random_number2);
}

int main() {

	//main window

	sf::RenderWindow window(sf::VideoMode(1100, 600), "Game project v1");
	sf::Clock clock;

	int animation_fps = 10;
	Hero hero(animation_fps);
	sf::Texture heroIdleTexture, heroWalkTextureLeft, heroWalkTextureRight, heroAttackTexture;
	setupHero(hero, animation_fps);

	std::vector<sf::Texture> enemyTextures(4);
	std::vector<Enemy> enemies(4, Enemy(animation_fps));

	setupEnemy(enemies[0], enemyTextures[0], "enemy1_Idle.png", 1.5, sf::Vector2f(300, 200), { {0, 0, 128, 128},{128, 0, 128, 128},{256, 0, 128, 128},{384, 0, 128, 128},{512, 0, 128, 128},{640, 0, 128, 128},{768, 0, 128, 128}
		});

	setupEnemy(enemies[1], enemyTextures[1], "enemy2_Idle.png", 1.5, sf::Vector2f(670, 30), { {0, 0, 128, 128},{128, 0, 128, 128},{256, 0, 128, 128},{384, 0, 128, 128},{512, 0, 128, 128}
		});

	setupEnemy(enemies[2], enemyTextures[2], "enemy3_Idle.png", 2, sf::Vector2f(520, 200), { {0, 0, 96, 96},{96, 0, 96, 96},{192, 0, 96, 96},{288, 0, 96, 96},{384, 0, 96, 96}
		});

	setupEnemy(enemies[3], enemyTextures[3], "enemy4_Idle.png", 2, sf::Vector2f(800, 210), { {0, 0, 128, 128},{128, 0, 128, 128},{256, 0, 128, 128},{384, 0, 128, 128},{512, 0, 128, 128}
		});

	//main textures
	// main map
	sf::Texture map_texture;
	loadTexture(map_texture, "mapka0.png");
	sf::Sprite map(map_texture);
	map.setScale(0.5, 0.5);

	//quest1 map
	sf::Texture map1_texture;
	loadTexture(map1_texture, "mapka1.png");
	sf::Sprite map1(map1_texture);
	map1.setScale(1, 1);
	
	//quest 3 map
	sf::Texture map3;
	loadTexture(map3, "mapka3.png");
	sf::Sprite map3Sprite(map3);
	map3Sprite.setScale(0.5, 0.5);


	//quest 1 
	sf::Texture questTexture;
	loadTexture(questTexture, "quest.png");
	sf::Sprite questSprite(questTexture);
	questSprite.setPosition(370, 220);
	questSprite.setScale(0.3, 0.3);

	//quest 2
	sf::Texture quest2Texture;
	loadTexture(quest2Texture, "quest2.png");
	sf::Sprite quest2Sprite(quest2Texture);
	quest2Sprite.setPosition(900, 250);
	quest2Sprite.setScale(0.3, 0.3);

	// quest 3
	sf::Texture quest3Texture;
	loadTexture(quest3Texture, "quest3.png");
	sf::Sprite quest3Sprite(quest3Texture);
	quest3Sprite.setPosition(750, 90);
	quest3Sprite.setScale(0.3, 0.3);

	// end game
	sf::Texture end_game;
	loadTexture(end_game, "end_game.png");
	sf::Sprite endgame(end_game);
	endgame.setPosition(250, 80);

	sf::Texture projectileTexture;
	loadTexture(projectileTexture, "projectile.png");
	std::vector<Projectile> projectiles;

	sf::RectangleShape healthBarBackground(sf::Vector2f(200, 20));
	healthBarBackground.setFillColor(sf::Color(50, 50, 50));
	healthBarBackground.setPosition(10, 570);

	sf::RectangleShape healthBar(sf::Vector2f(200, 20));
	healthBar.setFillColor(sf::Color(100, 250, 50));
	healthBar.setPosition(10, 570);
	//
	sf::RectangleShape healthBarEnemyBackground(sf::Vector2f(200, 20));
	healthBarEnemyBackground.setFillColor(sf::Color(50, 50, 50));
	healthBarEnemyBackground.setPosition(850, 30);

	sf::RectangleShape healthBarEnemy(sf::Vector2f(200, 20));
	healthBarEnemy.setFillColor(sf::Color::Yellow);
	healthBarEnemy.setPosition(850, 30);

	sf::Clock projectileClock;
	float fireInterval = 2.0f;

	bool quest1 = true;
	bool quest2 = false;
	bool quest3 = false;
	bool _endgame = false;


	//main window

	while (window.isOpen()) {

		sf::Event event;

		while (window.pollEvent(event)) {
			//closing 
			if (event.type == sf::Event::Closed) {
				window.close();
			}


			if (event.type == sf::Event::MouseButtonPressed) {

				//hero attacking on right click
				if (event.mouseButton.button == sf::Mouse::Right) {
					hero.attack();
				}

				//open quest on left click
				if (event.mouseButton.button == sf::Mouse::Left) {
					sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
					if (questSprite.getGlobalBounds().contains(mousePosition)) {

						//quest 1 subWindow
						sf::RenderWindow subWindow(sf::VideoMode(950, 650), "Quest Window");
						Hero subHero(10);
						setupHero(subHero, animation_fps);
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

						//closing window by sprite
						sf::Texture end1Texture;
						loadTexture(end1Texture, "endquest1.png");
						sf::Sprite end1Sprite(end1Texture);
						end1Sprite.setPosition(270, 130);

						//quest 1 begin
						while (subWindow.isOpen()) {


							sf::Event subEvent;

							while (subWindow.pollEvent(subEvent)) {
								if (subEvent.type == sf::Event::Closed) {
									subWindow.close();
								}

								if (subEvent.type == sf::Event::MouseButtonPressed) {
									if (subEvent.mouseButton.button == sf::Mouse::Right) {
										subHero.attack();
									}

									if (subEvent.mouseButton.button == sf::Mouse::Left) {
										sf::Vector2f mousePosition = subWindow.mapPixelToCoords(sf::Vector2i(subEvent.mouseButton.x, subEvent.mouseButton.y));
										if (end1Sprite.getGlobalBounds().contains(mousePosition) && end1 == true) {
											subWindow.close();

										}
									}
								}

								//stone 1
								if (subHero.getPosition().x >= 200 && subHero.getPosition().x <= 220 && subHero.getPosition().y >= 185 && subHero.getPosition().y <= 255 && check == true) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stoneVisible = false;
										check = false;
									}
								}

								if (subHero.getPosition().x >= 300 && subHero.getPosition().x <= 340 && subHero.getPosition().y >= 185 && subHero.getPosition().y <= 215 && check == false) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stoneSprite.setPosition(370, 250);
										stoneSprite.setScale(3, 3);
										if (stoneVisible == false) {
											it++;
										}
										stoneVisible = true;
										std::cout << it << std::endl;

									}
								}

								//stone 2 - 130,150
								if (subHero.getPosition().x >= 60 && subHero.getPosition().x <= 80 && subHero.getPosition().y >= 75 && subHero.getPosition().y <= 95&& check2 == true) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stoneVisible2 = false;
										check2 = false;
									}
								}

								if (subHero.getPosition().x >= 340 && subHero.getPosition().x <= 380 && subHero.getPosition().y >= 185 && subHero.getPosition().y <= 215 && check2 == false) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stone2Sprite.setPosition(410, 250);
										stone2Sprite.setScale(3, 3);
										if (stoneVisible2 == false) {
											it++;
										}
										stoneVisible2 = true;
										std::cout << it << std::endl;
									}
								}
								//stone 3 - 330,350
								if (subHero.getPosition().x >= 260 && subHero.getPosition().x <= 280 && subHero.getPosition().y >= 275 && subHero.getPosition().y <= 295 && check3 == true) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stoneVisible3 = false;
										check3 = false;
									}
								}

								if (subHero.getPosition().x >= 380 && subHero.getPosition().x <= 420 && subHero.getPosition().y >= 185 && subHero.getPosition().y <=215 && check3 == false) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stone3Sprite.setPosition(450, 250);
										stone3Sprite.setScale(3, 3);
										if (stoneVisible3 == false) {
											it++;
										}
										stoneVisible3 = true;
										std::cout << it << std::endl;
									}
								}
								//stone 4 - 600,300
								if (subHero.getPosition().x >= 530 && subHero.getPosition().x <= 550 && subHero.getPosition().y >= 225 && subHero.getPosition().y <= 245 && check4 == true) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stoneVisible4 = false;
										check4 = false;
									}
								}

								if (subHero.getPosition().x >= 340 && subHero.getPosition().x <= 380 && subHero.getPosition().y >= 225 && subHero.getPosition().y <= 255 && check4 == false) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stone4Sprite.setPosition(410, 290);
										stone4Sprite.setScale(3, 3);
										if (stoneVisible4 == false) {
											it++;
										}
										stoneVisible4 = true;
										std::cout << it << std::endl;
									}
								}
								//stone 5 - 620,450
								if (subHero.getPosition().x >= 550 && subHero.getPosition().x <= 570 && subHero.getPosition().y >= 375 && subHero.getPosition().y <= 395 && check5 == true) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stoneVisible5 = false;
										check5 = false;
									}
								}

								if (subHero.getPosition().x >= 340 && subHero.getPosition().x <= 380 && subHero.getPosition().y >= 265 && subHero.getPosition().y <= 295 && check5 == false) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stone5Sprite.setPosition(410, 330);
										stone5Sprite.setScale(3, 3);
										if (stoneVisible5 == false) {
											it++;
										}
										stoneVisible5 = true;
										std::cout << it << std::endl;
									}
								}
								//stone 6 - 180,400
								if (subHero.getPosition().x >= 110 && subHero.getPosition().x <= 130 && subHero.getPosition().y >= 325 && subHero.getPosition().y <=345 && check6 == true) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stoneVisible6 = false;
										check6 = false;
									}
								}

								if (subHero.getPosition().x >= 380 && subHero.getPosition().x <= 420 && subHero.getPosition().y >= 265 && subHero.getPosition().y <= 295 && check6 == false) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stone6Sprite.setPosition(450, 330);
										stone6Sprite.setScale(3, 3);
										if (stoneVisible6 == false) {
											it++;
										}
										stoneVisible6 = true;
										std::cout << it << std::endl;
									}
								}
								//stone 7 - 300,200
								if (subHero.getPosition().x >= 230 && subHero.getPosition().x <= 250 && subHero.getPosition().y >= 125 && subHero.getPosition().y <= 145 && check7 == true) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stoneVisible7 = false;
										check7 = false;
									}
								}

								if (subHero.getPosition().x >= 300 && subHero.getPosition().x <= 340 && subHero.getPosition().y >= 265 && subHero.getPosition().y <= 295 && check7 == false) {

									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
										stone7Sprite.setPosition(370, 330);
										stone7Sprite.setScale(3, 3);
										if (stoneVisible7 == false) {
											it++;
										}
										stoneVisible7 = true;
										std::cout << it << std::endl;
									}
								}

								if (it == 7) {
									end1 = true;
									quest2 = true;
									quest1 = false;
								}

							}

							float delta_time = clock.restart().asSeconds();
							float hero_speed = 0.1;

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


					//quest 2 opening by sprite
					if (quest2Sprite.getGlobalBounds().contains(mousePosition)) {

						//quest 2 window
						sf::RenderWindow subWindow(sf::VideoMode(1100, 600), "Quest II Window");
						Hero subHero(10);
						setupHero(subHero, animation_fps);
						sf::FloatRect windowBounds(0, 0, subWindow.getSize().x, subWindow.getSize().y);

						Enemy subEnemy(animation_fps);
						setupEnemy(subEnemy, enemyTextures[3], "enemy4_Idle.png", 2, sf::Vector2f(800, 210), {
							{0, 0, 128, 128},
							{128, 0, 128, 128},
							{256, 0, 128, 128},
							{384, 0, 128, 128},
							{512, 0, 128, 128}
							});

						subEnemy.setMovement(200, 100, 1);
						subEnemy.startMovement(sf::Vector2f(800, 210));

						sf::Clock subClock;

						sf::Texture end2Texture;
						loadTexture(end2Texture, "endquest2.png");
						sf::Sprite end2Sprite(end2Texture);
						end2Sprite.setPosition(270, 130);

						sf::Texture fail2Texture;
						loadTexture(fail2Texture, "failquest2.png");
						sf::Sprite fail2Sprite(fail2Texture);
						fail2Sprite.setPosition(270, 130);

						bool enemy_dead = false;
						bool end2 = false;
						bool enemySpawn = true;

						while (subWindow.isOpen()) {

							sf::Event subEvent;
							while (subWindow.pollEvent(subEvent)) {
								if (subEvent.type == sf::Event::Closed) {
									subWindow.close();
								}
								if (subEvent.type == sf::Event::MouseButtonPressed) {
									if (subEvent.mouseButton.button == sf::Mouse::Right) {
										subHero.attack();
									}
									if (subEvent.mouseButton.button == sf::Mouse::Left) {
										sf::Vector2f mousePosition = subWindow.mapPixelToCoords(sf::Vector2i(subEvent.mouseButton.x, subEvent.mouseButton.y));
										if (end2Sprite.getGlobalBounds().contains(mousePosition)) {
											subWindow.close();

										}
									}
								}
								sf::Vector2f mousePosition = subWindow.mapPixelToCoords(sf::Vector2i(subEvent.mouseButton.x, subEvent.mouseButton.y));
								sf::FloatRect subHeroBounds = subHero.getGlobalBounds();
								sf::FloatRect subEnemyBounds = subEnemy.getGlobalBounds();

								if (subHero.getPosition().x >= 780 && subHero.getPosition().x <= 820 && subHeroBounds.intersects(subEnemyBounds) && subEvent.mouseButton.button == sf::Mouse::Right) {
									healthBarEnemy.setFillColor(sf::Color::Red);
									end2 = true;
									quest3 = true;
									quest2 = false;
									enemySpawn = false;

								}
							}

							float delta_time = subClock.restart().asSeconds();
							float hero_speed = 0.1;

							sf::Vector2f movement = handleMovement(subHero, hero_speed);
							sf::Vector2f hero_pos = subHero.getPosition();
							sf::FloatRect hero_bounds = subHero.getGlobalBounds();

							checkBounds(movement, hero_pos, windowBounds, hero_bounds);
							subHero.move(movement);
							subHero.update(delta_time);

							subEnemy.update(delta_time);

							if (projectileClock.getElapsedTime().asSeconds() >= fireInterval) {
								sf::Vector2f enemy_center = subEnemy.getPosition() + sf::Vector2f(subEnemy.getGlobalBounds().width / 2, subEnemy.getGlobalBounds().height / 2);
								projectiles.push_back(Projectile(projectileTexture, enemy_center, 300.0f));
								projectileClock.restart();
							}

							for (auto it = projectiles.begin(); it != projectiles.end();) {
								it->update(delta_time);

								if (it->getGlobalBounds().intersects(hero_bounds)) {
									subHero.decreaseHealth(20);
									it = projectiles.erase(it);
								}
								else if (it->isOffScreen(windowBounds)) {
									it = projectiles.erase(it);
								}
								else {
									++it;
								}
							}

							float healthPercent = static_cast<float>(subHero.getHealth()) / 100.0f;
							healthBar.setSize(sf::Vector2f(200 * healthPercent, 20));

							if (subHero.getHealth() <= 0) {
								subWindow.clear();
								subWindow.draw(fail2Sprite);
								subWindow.display();

								while (true) {
									if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
										subWindow.close();
										break;
									}
								}
							}
							subWindow.clear();
							subWindow.draw(map);
							subWindow.draw(healthBarEnemyBackground);
							subWindow.draw(healthBarEnemy);
							subWindow.draw(healthBarBackground);
							subWindow.draw(healthBar);
							subWindow.draw(subHero);
							

							subWindow.draw(subEnemy);
							if (enemySpawn) {
								for (const auto& projectile : projectiles) {
									subWindow.draw(projectile);
								}
							}
							if (end2)
							{
								subWindow.draw(end2Sprite);

							}
							subWindow.display(); 
						}


					}

					//quest 3 opening by sprite
					if (quest3Sprite.getGlobalBounds().contains(mousePosition)) {

						sf::RenderWindow subWindow(sf::VideoMode(1100, 600), "Quest III Window");
						Hero subHero(1);
						setupHero(subHero, animation_fps);
						sf::FloatRect windowBounds(0, 0, subWindow.getSize().x, subWindow.getSize().y);

						sf::Texture end3Texture;
						loadTexture(end3Texture, "endquest3.png");
						sf::Sprite end3Sprite(end3Texture);
						end3Sprite.setPosition(270, 130);


						sf::Texture bulletTexture;
						loadTexture(bulletTexture, "coin.png");

						std::vector<Bullet> bullets;
						const float bulletSpeed = 2.0f;

						bool end3 = false;
						float bulletSpawnTimer = 0.0f;
						float delta_time = clock.restart().asSeconds();
						int prize = 0;

						//quest 3 window
						while (subWindow.isOpen()) {

							if (bulletSpawnTimer - (bulletSpawnTimer / 20.0f) * 20.0f <= 0.0f && bullets.size() < 10) {


								sf::Vector2f bulletDirection = getRandomPosition(-1.0f, 1.0f);
								sf::Vector2f bulletPos = getRandomPosition(0.0f, 1000.0f);

								bullets.emplace_back(bulletTexture, bulletPos, bulletDirection, bulletSpeed);
							}

							bulletSpawnTimer += 0.05f;

							sf::Event subEvent;
							while (subWindow.pollEvent(subEvent)) {
								if (subEvent.type == sf::Event::Closed) {
									subWindow.close();
								}

								if (subEvent.type == sf::Event::MouseButtonPressed) {
									if (subEvent.mouseButton.button == sf::Mouse::Right) {
										subHero.attack();
									}
									if (subEvent.mouseButton.button == sf::Mouse::Left) {
										sf::Vector2f mousePosition = subWindow.mapPixelToCoords(sf::Vector2i(subEvent.mouseButton.x, subEvent.mouseButton.y));
										if (end3Sprite.getGlobalBounds().contains(mousePosition) && end3 == true) {
											subWindow.close();

										}
									}
								}
							}

							float hero_speed = 0.1;
							sf::Vector2f movement = handleMovement(subHero, hero_speed);
							sf::Vector2f hero_pos = subHero.getPosition();
							sf::FloatRect hero_bounds = subHero.getGlobalBounds();

							checkBounds(movement, hero_pos, windowBounds, hero_bounds);
							subHero.move(movement);
							subHero.update(delta_time);

							for (auto it = bullets.begin(); it != bullets.end();) {
								it->update(delta_time);

								if (it->getBounds().intersects(subHero.getGlobalBounds())) {
									prize++;
									std::cout << "prize: " <<prize<< std::endl;
									it = bullets.erase(it);
									if (prize >= 10) {
										end3 = true;
										quest3 = false;
										_endgame = true;
									}
								}
								else {
									++it;
								}
							}
							bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [&windowBounds](const Bullet& bullet) {
								return !windowBounds.intersects(bullet.getBounds());
								}), bullets.end());

							for (Enemy& enemy : enemies) {
								enemy.update(delta_time);
							}

							subWindow.clear();
							subWindow.draw(map3Sprite);
							for (const auto& bullet : bullets) {
								bullet.draw(subWindow);
							}
							subWindow.draw(subHero);
							if (end3 && prize >= 10)
							{
								subWindow.draw(end3Sprite);
							}

							subWindow.display();
						}
					}
				}
			}
		}



		float delta_time = clock.restart().asSeconds();
		float hero_speed = 0.1;

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
		if (quest1) {
			window.draw(questSprite);
		}
		if (quest2) {
			window.draw(quest2Sprite);
		}
		if (quest3) {
			window.draw(quest3Sprite);
		}

		window.draw(hero);
		if (_endgame) {
			window.draw(endgame);
		}
		window.display();
	}


	return 0;
}