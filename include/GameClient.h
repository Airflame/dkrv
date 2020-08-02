#ifndef DKRV_GAMECLIENT_H
#define DKRV_GAMECLIENT_H
#define WINNERTEXT_INTERVAL 3
#include <SFML/Network.hpp>
#include "Constants.h"
#include "Player.h"
#include "WinnerText.h"
#include "EffectFast.h"
#include "EffectSlow.h"

class GameClient {
private:
    sf::TcpSocket server;
    sf::RenderWindow* window;
    std::vector<sf::Texture> textures;
    std::vector<std::string> names;
    std::vector<sf::Color> colors;
    std::vector<Player> players;
    std::vector<Effect*> effects;
    WinnerText winnerText;
    bool listening, running, drawWinnerText;
    void connect();
    void netLoop();
public:
    GameClient();
    void run();
};


#endif //DKRV_GAMECLIENT_H
