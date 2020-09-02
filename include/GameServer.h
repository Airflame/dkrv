#ifndef DKRV_GAMESERVER_H
#define DKRV_GAMESERVER_H
#include <windows.h>
#include <SFML/Network.hpp>
#include "Constants.h"
#include "Player.h"
#include "EffectFast.h"
#include "EffectSlow.h"


class GameServer {
private:
    sf::RenderWindow* window;
    sf::SocketSelector selector;
    sf::Packet sendPacket;
    std::vector<sf::TcpSocket*> clients;
    std::map<unsigned short, int> ids;
    std::vector<std::string> names;
    std::vector<Player> players;
    std::vector<sf::Color> colors;
    std::vector<bool> playerTurns, playerTurnsLeft;
    std::vector<Effect*> effects;
    bool listening, running;
    void listen();
    void netLoop();
    void sendPositions();
    void sendStartGame();
    void sendWinner(int wonId);
    void sendNextRound();
    void sendNewEffect(int effectType, float x, float y);
    void sendEffectCollected(int effectId, int playerId);
    void sendTurn(int playerId, bool turn, bool turnLeft);
    void sendDrawing(int playerId, bool drawing);
    void sendBlocked(int playerId);
    void sendSynchronization();

public:
    GameServer();
    void run();
};


#endif //DKRV_GAMESERVER_H
