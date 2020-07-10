#ifndef DKRV_PLAYER_H
#define DKRV_PLAYER_H
#include <cmath>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>


class Player {
private:
    std::vector<Player>* players;
    sf::Vector2f position;
    float velocity;
    float angle;
    bool isDrawing;
    std::vector<sf::Vector2f> line;
    sf::CircleShape pointShape;
    sf::Color color;
    bool isCollision();
public:
    Player();
    void clear();
    void setEnemies(std::vector<Player>* arg);
    void drawLine(sf::RenderWindow* window);
    void move(float dt);
    void turnLeft(float dt);
    void turnRight(float dt);
    void addPosition(sf::Vector2f arg);
    sf::Vector2f getPosition();
    void setColor(sf::Color arg);
    void enableDrawing();
};


#endif //DKRV_PLAYER_H
