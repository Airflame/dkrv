#include "../include/Player.h"

#include <cmath>

Player::Player() {
    srand(time(nullptr));
    velocity = PLAYER_VELOCITY;
    color = sf::Color::White;
    drawing = isInClient = false;
    pointShape.setRadius(PLAYER_RADIUS);
    pointShape.setOrigin(PLAYER_RADIUS, PLAYER_RADIUS);
    reset();
}

void Player::draw(sf::RenderWindow* window) {
    pointShape.setFillColor(color);
    for (auto point : line) {
        pointShape.setPosition(point);
        window->draw(pointShape);
    }
    pointShape.setPosition(position);
    window->draw(pointShape);
}

void Player::reset() {
    clear();
    setAngle(0);
    started = false;
    holeTimer = 2 * HOLE_TIMER;
    drawingHole = false;
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
    if (!isInClient) {
        if (line.size() < 25 / velocityModifier)
            return false;
        for (int i = 0; i < line.size() - 25 / velocityModifier; i++) {
            sf::Vector2f point = line[i];
            if (std::sqrt(
                    (point.x - position.x) * (point.x - position.x) + (point.y - position.y) * (point.y - position.y)) <
                2 * PLAYER_RADIUS)
                return true;
        }
        for (const auto &enemy : *players) {
            for (auto point : enemy.line) {
                if (enemy.color == this->color)
                    continue;
                if (std::sqrt(
                        (point.x - position.x) * (point.x - position.x) +
                        (point.y - position.y) * (point.y - position.y)) <
                    2 * PLAYER_RADIUS)
                    return true;
            }
        }
        if (drawing)
            return position.x < PLAYER_RADIUS or position.x > WINDOW_SIZE - PLAYER_RADIUS or
                   position.y < PLAYER_RADIUS or
                   position.y > WINDOW_SIZE - PLAYER_RADIUS;
        else
            return false;
    } else
        return false;
}

void Player::move(float dt) {
    if (!blocked)
        blocked = isCollision() and drawing;
    if (!blocked) {
        position.x += velocity * std::cos(angle) * dt * velocityModifier;
        position.y += velocity * std::sin(angle) * dt * velocityModifier;
        if (drawing)
            line.emplace_back(position);
    }
}

int Player::makeHoles(float dt) {
    if (drawing and holeTimer <= 0 and rand() % HOLE_FREQ == 0) {
        disableDrawing();
        holeTimer = 2 * HOLE_TIMER;
        drawingHole = true;
        return DRAWING_DISABLED;
    }
    if (drawingHole and holeTimer <= HOLE_TIMER) {
        enableDrawing();
        drawingHole = false;
        return DRAWING_ENABLED;
    }
    if (started and holeTimer > 0)
        holeTimer -= dt * velocityModifier;
    return 0;
}

void Player::turnLeft(float dt) {
    angle -= 3 * dt * turningModifier;
    if (angle < 0)
        angle += 2 * M_PI;
}

void Player::turnRight(float dt) {
    angle += 3 * dt * turningModifier;
    if (angle > 2 * M_PI)
        angle -= 2 * M_PI;
}

void Player::setPosition(sf::Vector2f arg) {
    position = arg;
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

void Player::resetModifiers() {
    velocityModifier = turningModifier = 1;
}

void Player::setAsClient() {
    isInClient = true;
}

void Player::setBlocked(bool arg) {
    blocked = arg;
}

bool Player::isDrawing() const {
    return drawing;
}

bool Player::isBlocked() const {
    return blocked;
}

void Player::setAngle(float arg) {
    angle = arg;
}

float Player::getAngle() {
    return angle;
}
