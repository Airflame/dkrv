#include "include/GameServer.h"
#include "include/GameClient.h"

int main() {
    bool isServer;
    std::cout << "Run server[1] or client[0]?" << std::endl;
    std::cin >> isServer;
    if (isServer) {
        GameServer gameServer;
        gameServer.run();
    } else {
        GameClient gameClient;
        gameClient.run();
    }
    return 0;
}
