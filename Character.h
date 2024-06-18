#include <SFML/Graphics.hpp>
#include <vector>

class Character : public sf::Sprite {
public:
    Character(int fps);
    Character();
    virtual void attack() = 0;
    virtual void setIdleTexture(const sf::Texture& texture);
    virtual void setWalkTextureLeft(const sf::Texture& texture);
    virtual void setWalkTextureRight(const sf::Texture& texture);
    virtual void setAttackTexture(const sf::Texture& texture);
    virtual void add_idle_frame(const sf::IntRect& rect);
    virtual void add_walk_frame_left(const sf::IntRect& rect);
    virtual void add_walk_frame_right(const sf::IntRect& rect);
    virtual void add_attack_frame(const sf::IntRect& rect);
    virtual void setMoving(bool moving, bool movingLeft);
    virtual void setAttacking(bool isAttacking);
    virtual void update(float delta_time);

    void decreaseHealth(int amount);
    int getHealth() const;

    int animation_fps;
    int current_frame;
    float elapsed_time;
    float time_per_frame;
    bool is_moving;
    bool is_moving_left;
    bool attacking;
    int health;
    int dmg;

    std::unique_ptr<sf::Texture> idle_texture;
    std::unique_ptr<sf::Texture> walk_texture_left; 
    std::unique_ptr<sf::Texture> walk_texture_right;
    std::unique_ptr<sf::Texture> attackTexture;

    std::vector<sf::IntRect> idle_frames;
    std::vector<sf::IntRect> walk_frames_left;
    std::vector<sf::IntRect> walk_frames_right;
    std::vector<sf::IntRect> attack_frames;
    std::vector<sf::IntRect>* current_frames;
};
