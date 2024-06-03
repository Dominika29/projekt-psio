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
    questSprite.setScale(0.4,0.4);

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

                        while (subWindow.isOpen()) {
                            sf::Event subEvent;
                            while (subWindow.pollEvent(subEvent)) {
                                if (subEvent.type == sf::Event::Closed) {
                                    subWindow.close();
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
                            // Rysowanie t�a, wrog�w itp.
                            subWindow.draw(map1);
                            subWindow.draw(subHero);
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