#ifndef DKRV_GAME_H
#define DKRV_GAME_H
#include <iostream>
#include <SFML/Network.hpp>
#include "Player.h"


class Game {
private:
    sf::TcpSocket socket;
    bool isListening;
    void listen();
    void connect();
    void netLoop();
public:
    Game();
    Player player, enemy;
    void run(bool isServer);
};


#endif
