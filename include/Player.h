#ifndef DKRV_PLAYER_H
#define DKRV_PLAYER_H
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>


class Player {
private:
    Player* enemy;
    sf::Vector2f position;
    float velocity;
    float angle;
    std::vector<sf::Vector2f> line;
    sf::CircleShape pointShape;
    sf::Color color;
    bool isCollision();
public:
    Player();
    void setEnemy(Player* arg);
    void drawLine(sf::RenderWindow* window);
    void drawHead(sf::RenderWindow* window);
    void move(float dt);
    void turnLeft(float dt);
    void turnRight(float dt);
    void addPosition(sf::Vector2f arg);
    void setPosition(sf::Vector2f arg);
    sf::Vector2f getPosition();
    void setColor(sf::Color arg);
};


#endif //DKRV_PLAYER_H
