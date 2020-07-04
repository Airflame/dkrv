#ifndef DKRV_GAME_H
#define DKRV_GAME_H
#include <iostream>
#include "Player.h"


class Game {
private:
    void netLoop();
public:
    Game();
    void run();
};


#endif
