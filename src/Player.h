#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    void handleInput();
    void update(sf::Time deltaTime);
    void render(sf::RenderWindow& window);

private:
    sf::Sprite sprite;
    sf::Texture texture;
    float speed;
    float maxSpeed;
    float accelerationRate;
    float decelerationRate;
    sf::Vector2f velocity;

    enum Direction {
        Down,
        Left,
        Right,
        Up,
        None
    } direction;
    Direction lastDirection;

    void updateAnimation(sf::Time deltaTime);
    sf::IntRect getAnimationFrame(Direction dir, int frame);
    int currentFrame;
    sf::Time frameTime;
    sf::Time elapsedTime;
};