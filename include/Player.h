#ifndef DKRV_PLAYER_H
#define DKRV_PLAYER_H
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>


class Player {
private:
    sf::Vector2f position;
    float velocity;
    float angle;
    std::vector<sf::Vector2f> line;
public:
    Player();
    void drawLine(sf::RenderWindow* window);
    void drawHead(sf::RenderWindow* window);
    void move(float dt);
    void turnLeft(float dt);
    void turnRight(float dt);
};


#endif //DKRV_PLAYER_H
