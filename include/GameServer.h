#ifndef DKRV_GAMESERVER_H
#define DKRV_GAMESERVER_H
#include <windows.h>
#include <SFML/Network.hpp>
#include "Constants.h"
#include "Player.h"


class GameServer {
private:
    sf::SocketSelector selector;
    sf::Packet sendPacket;
    std::vector<sf::TcpSocket*> clients;
    std::map<unsigned short, int> ids;
    std::vector<std::string> names;
    std::vector<Player> players;
    std::vector<sf::Color> colors;
    bool listening, running;
    void listen();
    void netLoop();
    void sendPosition(int playerId);
    void sendStartGame();
    void sendWinner(int wonId);
    void sendNextRound();

public:
    GameServer();
    void run();
};


#endif //DKRV_GAMESERVER_H
