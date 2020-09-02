#ifndef DKRV_PLAYER_H
#define DKRV_PLAYER_H
#include <cmath>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Constants.h"


class Player {
private:
    std::vector<Player>* players;
    std::vector<sf::Vector2f> line;
    sf::Vector2f position;
    float velocity, angle, holeTimer;
    bool started, drawing, drawingHole, blocked, isInClient;
    sf::Color color;
    sf::CircleShape pointShape;
    bool isCollision();
public:
    float velocityModifier = 1, turningModifier = 1;
    Player();
    void draw(sf::RenderWindow* window);
    void reset();
    void clear();
    void start();
    void setEnemies(std::vector<Player>* arg);
    void move(float dt);
    int makeHoles(float dt);
    void turnLeft(float dt);
    void turnRight(float dt);
    void setPosition(sf::Vector2f arg);
    sf::Vector2f getPosition();
    void setColor(sf::Color arg);
    void enableDrawing();
    void disableDrawing();
    void resetModifiers();
    void setAsClient();
    void setBlocked(bool arg);
    void setAngle(float arg);
    float getAngle();
    bool isDrawing() const;
    bool isBlocked() const;
};


#endif //DKRV_PLAYER_H
