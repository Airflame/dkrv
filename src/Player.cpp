#include "../include/Player.h"

#include <cmath>

Player::Player() {
    position = sf::Vector2f(100, 100);
    velocity = 100;
    angle = 0;
    line.emplace_back(position);
}

void Player::drawLine(sf::RenderWindow *window) {
    sf::CircleShape pointShape(5);
    pointShape.setFillColor(sf::Color::Yellow);
    for (sf::Vector2f point : line) {
        pointShape.setPosition(point);
        window->draw(pointShape);
    }
}

void Player::drawHead(sf::RenderWindow *window) {
    sf::CircleShape pointShape(5);
    pointShape.setFillColor(sf::Color::Yellow);
    pointShape.setPosition(position);
    window->draw(pointShape);
}

void Player::move(float dt) {
    if (!isCollision()) {
        position.x += velocity * std::cos(angle) * dt;
        position.y += velocity * std::sin(angle) * dt;
        line.emplace_back(position);
    }
}

void Player::turnLeft(float dt) {
    angle -= 3*dt;
    if (angle < 0)
        angle += 2*M_PI;
}

void Player::turnRight(float dt) {
    angle += 3*dt;
    if (angle > 2*M_PI)
        angle -= 2*M_PI;
}

bool Player::isCollision() {
    if (line.size() < 10)
        return false;
    for (int i = 0; i < line.size() - 10; i++) {
        sf::Vector2f point = line[i];
        if (std::sqrt((point.x - position.x)*(point.x - position.x)+(point.y - position.y)*(point.y - position.y)) < 10)
            return true;
    }
    return false;
}


