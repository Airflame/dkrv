#include "include/GameServer.h"
#include "include/GameClient.h"

int main() {
    std::string isServer;
    std::cout << "Run [s]erver or [c]lient?" << std::endl;
    std::cin >> isServer;
    if (isServer == "s") {
        GameServer gameServer;
        gameServer.run();
    } else {
        GameClient gameClient;
        gameClient.run();
    }
    return 0;
}
