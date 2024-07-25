#include "Player.h"
#include <cmath>

const int FRAME_WIDTH = 28;
const int FRAME_HEIGHT = 34;
const int NUM_FRAMES = 3;
const sf::Time FRAME_DURATION = sf::seconds(0.15f);

Player::Player() 
    : speed(100.0f),  // Reduced speed for smoother movement
        maxSpeed(150.0f),  // Set a reasonable max speed
        accelerationRate(600.0f),  // Balanced acceleration
        decelerationRate(600.0f),  // Balanced deceleration
        velocity(0.0f, 0.0f),
        direction(None),
        lastDirection(Down),
        currentFrame(0),
        frameTime(FRAME_DURATION),
        elapsedTime(sf::Time::Zero) {
    if (!texture.loadFromFile("player.png")) {
        // Handle error
    }
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, FRAME_WIDTH, FRAME_HEIGHT));
    sprite.setPosition(400, 300);  // Start position
}

void Player::handleInput(sf::Time deltaTime) {
    sf::Vector2f acceleration(0.0f, 0.0f);  // Reset acceleration

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        direction = Up;
        acceleration.y = -accelerationRate;
    } 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        direction = Down;
        acceleration.y = accelerationRate;
    } 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        direction = Left;
        acceleration.x = -accelerationRate;
    } 
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        direction = Right;
        acceleration.x = accelerationRate;
    }

    if (acceleration.x != 0 || acceleration.y != 0) {
        lastDirection = direction;
    }

    // Apply acceleration
    velocity += acceleration * deltaTime.asSeconds();

    // Normalize velocity for diagonal movement
    if (velocity.x != 0 && velocity.y != 0) {
        float factor = 1.25f;
        velocity.x *= factor;
        velocity.y *= factor;
    }

    // Clamp velocity to max speed
    if (std::abs(velocity.x) > maxSpeed) {
        velocity.x = maxSpeed * (velocity.x / std::abs(velocity.x));
    }
    if (std::abs(velocity.y) > maxSpeed) {
        velocity.y = maxSpeed * (velocity.y / std::abs(velocity.y));
    }

    // Apply deceleration when no input
    if (acceleration.x == 0) {
        if (velocity.x > 0) {
            velocity.x -= decelerationRate * deltaTime.asSeconds();
            if (velocity.x < 0) velocity.x = 0;
        } else if (velocity.x < 0) {
            velocity.x += decelerationRate * deltaTime.asSeconds();
            if (velocity.x > 0) velocity.x = 0;
        }
    }

    if (acceleration.y == 0) {
        if (velocity.y > 0) {
            velocity.y -= decelerationRate * deltaTime.asSeconds();
            if (velocity.y < 0) velocity.y = 0;
        } else if (velocity.y < 0) {
            velocity.y += decelerationRate * deltaTime.asSeconds();
            if (velocity.y > 0) velocity.y = 0;
        }
    }
}

void Player::update(sf::Time deltaTime) {
    handleInput(deltaTime);  // Ensure handleInput is called here to get deltaTime
    sprite.move(velocity * deltaTime.asSeconds());
    updateAnimation(deltaTime);
}

void Player::updateAnimation(sf::Time deltaTime) {
    elapsedTime += deltaTime;

    if (elapsedTime >= frameTime) {
        elapsedTime -= frameTime;
        currentFrame = (currentFrame + 1) % NUM_FRAMES;
        sprite.setTextureRect(getAnimationFrame(lastDirection, currentFrame));
    }

    // Reset to first frame if not moving
    if (velocity == sf::Vector2f(0.0f, 0.0f)) {
        currentFrame = 0;
        sprite.setTextureRect(getAnimationFrame(lastDirection, currentFrame));
    }
}

sf::IntRect Player::getAnimationFrame(Direction dir, int frame) {
    return sf::IntRect(frame * FRAME_WIDTH, dir * FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT);
}

void Player::render(sf::RenderWindow& window) {
    window.draw(sprite);
}