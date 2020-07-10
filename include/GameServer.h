#ifndef DKRV_GAMESERVER_H
#define DKRV_GAMESERVER_H
#define FPS 60
#include <iostream>
#include <vector>
#include <SFML/Network.hpp>
#include "Player.h"


class GameServer {
private:
    sf::SocketSelector selector;
    std::vector<sf::TcpSocket*> clients;
    std::map<unsigned short, std::string> names;
    std::vector<Player> players;
    std::vector<sf::Color> colors;
    bool listening, running;
    void listen();
    void netLoop();
public:
    GameServer();
    void run();
};


#endif //DKRV_GAMESERVER_H
