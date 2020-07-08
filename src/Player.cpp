#include "../include/Player.h"

#include <cmath>

Player::Player() {
    position = sf::Vector2f(rand() % 800, rand() % 800);
    line.emplace_back(position);
    velocity = 100;
    angle = 0;
    pointShape = sf::CircleShape(5);
    color = sf::Color::Blue;
    pointShape.setFillColor(color);
    pointShape.setOrigin(5, 5);
    isDrawing = false;
}


void Player::clear() {
    line.clear();
}

void Player::setEnemy(Player* arg) {
    enemy = arg;
}

void Player::drawLine(sf::RenderWindow *window) {
    if (isDrawing) {
        for (sf::Vector2f point : line) {
            pointShape.setPosition(point);
            window->draw(pointShape);
        }
    } else {
        pointShape.setPosition(position);
        window->draw(pointShape);
    }
}

void Player::move(float dt) {
    if (!isCollision()) {
        position.x += velocity * std::cos(angle) * dt;
        position.y += velocity * std::sin(angle) * dt;
        if (isDrawing)
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
    for (auto point : enemy->line) {
        if (std::sqrt((point.x - position.x)*(point.x - position.x)+(point.y - position.y)*(point.y - position.y)) < 10)
            return true;
    }
    if (isDrawing)
        return position.x < 5 or position.x > 795 or position.y < 5 or position.y > 795;
    else
        return false;
}

void Player::addPosition(sf::Vector2f arg) {
    position = arg;
    if (isDrawing)
        line.emplace_back(position);
}

sf::Vector2f Player::getPosition() {
    return position;
}

void Player::setColor(sf::Color arg) {
    color = arg;
    pointShape.setFillColor(color);
}

void Player::enableDrawing() {
    isDrawing = true;
}


