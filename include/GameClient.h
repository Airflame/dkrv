#ifndef DKRV_GAMECLIENT_H
#define DKRV_GAMECLIENT_H
#include <SFML/Network.hpp>
#include "Player.h"

class GameClient {
private:
    sf::TcpSocket server;
    sf::RenderWindow* window;
    std::vector<sf::Color> colors;
    std::vector<std::vector<sf::Vector2f>> lines;
    std::vector<sf::Vector2f> positions;
    sf::CircleShape pointShape;
    bool listening, running;
    void connect();
    void netLoop();
    void draw();
public:
    GameClient();
    void run();
};


#endif //DKRV_GAMECLIENT_H
