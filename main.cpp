#include "include/Game.h"

int main() {
    bool isServer;
    std::cout << "Run server[1] or client[0]?" << std::endl;
    std::cin >> isServer;
    Game game;
    game.run(isServer);
    return 0;
}
