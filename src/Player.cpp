#include "../include/Player.h"

#include <cmath>

Player::Player() {
    srand(time(nullptr));
    velocity = PLAYER_VELOCITY;
    color = sf::Color::White;
    drawing = false;
    reset();
}

void Player::reset() {
    clear();
    started = false;
    holeTimer = 2 * HOLE_TIMER;
    drawingHole = false;
    angle = 0;
    blocked = false;
    position = sf::Vector2f(100, rand() % 600 + 100);
    line.emplace_back(position);
}

void Player::clear() {
    line.clear();
}

void Player::start() {
    started = true;
}

void Player::setEnemies(std::vector<Player> *arg) {
    players = arg;
}

bool Player::isCollision() {
    if (line.size() < 10)
        return false;
    for (int i = 0; i < line.size() - 10; i++) {
        sf::Vector2f point = line[i];
        if (std::sqrt(
                (point.x - position.x) * (point.x - position.x) + (point.y - position.y) * (point.y - position.y)) < 10)
            return true;
    }
    for (const auto &enemy : *players) {
        for (auto point : enemy.line) {
            if (enemy.color == this->color)
                continue;
            if (std::sqrt(
                    (point.x - position.x) * (point.x - position.x) + (point.y - position.y) * (point.y - position.y)) <
                10)
                return true;
        }
    }
    if (drawing)
        return position.x < PLAYER_RADIUS or position.x > WINDOW_SIZE - PLAYER_RADIUS or position.y < PLAYER_RADIUS or
               position.y > WINDOW_SIZE - PLAYER_RADIUS;
    else
        return false;
}

void Player::move(float dt) {
    if (!blocked)
        blocked = isCollision() and drawing;
    if (!blocked) {
        makeHoles(dt);
        position.x += velocity * std::cos(angle) * dt;
        position.y += velocity * std::sin(angle) * dt;
        if (drawing)
            line.emplace_back(position);
    }
}

void Player::makeHoles(float dt) {
    if (drawing and holeTimer <= 0 and rand() % HOLE_FREQ == 0) {
        disableDrawing();
        holeTimer = 2 * HOLE_TIMER;
        drawingHole = true;
    }
    if (drawingHole and holeTimer <= HOLE_TIMER) {
        enableDrawing();
        drawingHole = false;
    }
    if (started and holeTimer > 0)
        holeTimer -= dt;
}

void Player::turnLeft(float dt) {
    angle -= 3 * dt;
    if (angle < 0)
        angle += 2 * M_PI;
}

void Player::turnRight(float dt) {
    angle += 3 * dt;
    if (angle > 2 * M_PI)
        angle -= 2 * M_PI;
}

sf::Vector2f Player::getPosition() {
    return position;
}

void Player::setColor(sf::Color arg) {
    color = arg;
}

void Player::enableDrawing() {
    drawing = true;
}

void Player::disableDrawing() {
    drawing = false;
}

bool Player::isDrawing() const {
    return drawing;
}

bool Player::isBlocked() const {
    return blocked;
}
