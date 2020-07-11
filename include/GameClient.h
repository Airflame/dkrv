#ifndef DKRV_GAMECLIENT_H
#define DKRV_GAMECLIENT_H
#define WINNERTEXT_INTERVAL 3
#include <SFML/Network.hpp>
#include "Player.h"
#include "WinnerText.h"

class GameClient {
private:
    sf::TcpSocket server;
    sf::RenderWindow* window;
    std::vector<std::string> names;
    std::vector<sf::Color> colors;
    std::vector<std::vector<sf::Vector2f>> lines;
    std::vector<sf::Vector2f> positions;
    sf::CircleShape pointShape;
    WinnerText winnerText;
    bool listening, running, drawWinnerText;
    void connect();
    void netLoop();
    void draw();
public:
    GameClient();
    void run();
};


#endif //DKRV_GAMECLIENT_H
