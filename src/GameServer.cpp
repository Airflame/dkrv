#include "../include/GameServer.h"

GameServer::GameServer() {
    colors.emplace_back(214, 48, 49);
    colors.emplace_back(253, 203, 110);
    colors.emplace_back(39, 174, 96);
    colors.emplace_back(9, 132, 227);
    colors.emplace_back(232, 67, 147);
    listening = true;
    running = true;
}

void GameServer::listen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int consoleColors[5] = {12, 14, 10, 9, 13};
    int clientId = 0, maxClients = 0, namesSet = 0;
    bool waiting = true;
    while (maxClients < 2 or maxClients > 5) {
        std::cout << "Number of players [2-5]:" << std::endl;
        std::cin >> maxClients;
    }
    sf::TcpListener listener;
    listener.listen(7777);
    selector.add(listener);
    while (waiting) {
        std::cout << "Waiting..." << std::endl;
        if (selector.wait()) {
            if (selector.isReady(listener)) {
                auto *client = new sf::TcpSocket;
                if (listener.accept(*client) == sf::Socket::Done) {
                    clients.push_back(client);
                    ids[client->getRemotePort()] = clientId;
                    selector.add(*client);
                    names.emplace_back("");
                    Player player;
                    player.setColor(colors[clientId]);
                    players.push_back(player);
                    SetConsoleTextAttribute(hConsole, consoleColors[clientId]);
                    std::cout << "New client (" << clientId << ") - " << client->getRemoteAddress() << std::endl;
                    SetConsoleTextAttribute(hConsole, 7);
                    clientId++;
                    if (clientId >= maxClients)
                        selector.remove(listener);
                } else
                    delete client;
            } else {
                for (sf::TcpSocket *socket : clients) {
                    sf::TcpSocket &client = *socket;
                    if (selector.isReady(client)) {
                        sf::Packet packet;
                        if (client.receive(packet) == sf::Socket::Done) {
                            packet >> names[ids[client.getRemotePort()]];
                            SetConsoleTextAttribute(hConsole, consoleColors[ids[client.getRemotePort()]]);
                            std::cout << "Client " << client.getRemoteAddress()
                                      << " nickname - " << names[ids[client.getRemotePort()]] << std::endl;
                            SetConsoleTextAttribute(hConsole, 7);
                            namesSet++;
                            if (namesSet >= maxClients)
                                waiting = false;
                        }
                    }
                }
            }
        }
    }
}

void GameServer::netLoop() {
    while (listening) {
        bool toLeft;
        if (selector.wait()) {
            for (int i = 0; i < clients.size(); i++) {
                sf::TcpSocket &client = *clients[i];
                if (selector.isReady(client)) {
                    sf::Packet packet;
                    if (client.receive(packet) == sf::Socket::Done) {
                        packet >> toLeft;
                        if (turn) {
                            if (toLeft)
                                players[i].turnLeft(1.0f / FPS);
                            else
                                players[i].turnRight(1.0f / FPS);
                            turn = false;
                        }
                    }
                }
            }
        }
    }
}

void GameServer::run() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int consoleColors[5] = {12, 14, 10, 9, 13};

    listen();
    std::cout << std::endl << "New game" << std::endl;
    sendStartGame();

    srand(time(nullptr));
    sf::Clock cl;
    float dt = 0;
    float startInterval = START_INTERVAL;
    float roundInterval = ROUND_INTERVAL;
    float refreshInterval = 1.0f / FPS;
    bool won = false, warmUp = true;
    int wonId;

    sf::Thread thread(&GameServer::netLoop, this);
    thread.launch();

    for (auto &player : players) {
        player.clear();
        player.setEnemies(&players);
    }

    while (running) {
        if (warmUp) {
            if (startInterval > 0)
                startInterval -= dt;
            else {
                for (auto &player : players) {
                    player.clear();
                    player.enableDrawing();
                    player.start();
                }
                warmUp = false;
            }
        }

        if (refreshInterval > 0)
            refreshInterval -= dt;
        else {
            refreshInterval = 1.0f / FPS;
            turn = true;
            int blocked = 0;
            for (int id = 0; id < players.size(); id++) {
                players[id].move(refreshInterval);
                sendPosition(id);
                if (players[id].isBlocked())
                    blocked++;
            }
            if (blocked >= players.size() - 1) {
                roundInterval -= refreshInterval;
                if (!won) {
                    won = true;
                    bool draw = true;
                    for (int i = 0; i < players.size(); i++) {
                        if (!players[i].isBlocked()) {
                            draw = false;
                            wonId = i;
                            SetConsoleTextAttribute(hConsole, consoleColors[i]);
                            std::cout << "Winner: " << names[i] << std::endl;
                            SetConsoleTextAttribute(hConsole, 7);
                            break;
                        }
                    }
                    if (draw) {
                        wonId = DRAW;
                        std::cout << "Draw" << std::endl;
                    }
                    sendWinner(wonId);
                }
            }
            if (roundInterval < 0) {
                for (auto &player : players) {
                    player.reset();
                    player.disableDrawing();
                }
                sendNextRound();
                won = false;
                warmUp = true;
                startInterval = START_INTERVAL;
                roundInterval = ROUND_INTERVAL;
                std::cout << "New game" << std::endl;
            }
        }
        dt = cl.restart().asSeconds();
    }
}

void GameServer::sendPosition(int playerId) {
    sf::Packet packet;
    packet.clear();
    packet << playerId << players[playerId].getPosition().x << players[playerId].getPosition().y
           << players[playerId].isDrawing();
    for (auto client : clients)
        client->send(packet);
}

void GameServer::sendStartGame() {
    sf::Packet packet;
    packet << ID_START_GAME;
    for (const auto &name : names)
        packet << name;
    for (auto client : clients)
        client->send(packet);
}

void GameServer::sendWinner(int wonId) {
    sf::Packet packet;
    packet.clear();
    packet << ID_SHOW_WINNER << wonId;
    for (auto client : clients)
        client->send(packet);
}

void GameServer::sendNextRound() {
    sf::Packet packet;
    packet.clear();
    packet << ID_NEXT_ROUND;
    for (auto client : clients)
        client->send(packet);
}

