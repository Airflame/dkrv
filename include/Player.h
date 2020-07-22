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
    sf::Vector2f position;
    float velocity, angle, holeTimer;
    bool started, drawing, drawingHole, blocked;
    std::vector<sf::Vector2f> line;
    sf::Color color;
    bool isCollision();
    void makeHoles(float dt);
public:
    Player();
    void reset();
    void clear();
    void start();
    void setEnemies(std::vector<Player>* arg);
    void move(float dt);
    void turnLeft(float dt);
    void turnRight(float dt);
    sf::Vector2f getPosition();
    void setColor(sf::Color arg);
    void enableDrawing();
    void disableDrawing();
    bool isDrawing() const;
    bool isBlocked() const;
};


#endif //DKRV_PLAYER_H
