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
                    turn.push_back(false);
                    toLeft.push_back(false);
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
        bool receivedTurn, receivedDirection;
        if (selector.wait()) {
            for (int i = 0; i < clients.size(); i++) {
                sf::TcpSocket &client = *clients[i];
                if (selector.isReady(client)) {
                    sf::Packet packet;
                    if (client.receive(packet) == sf::Socket::Done) {
                        packet >> receivedTurn;
                        turn[i] = receivedTurn;
                        if (turn[i]) {
                            packet >> receivedDirection;
                            toLeft[i] = receivedDirection;
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
    float startTimer = 0;
    float roundTimer = 0;
    const float refreshInterval = 1.0f / TPS;
    float refreshTimer = 0;
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
            if (startTimer < START_INTERVAL)
                startTimer += dt;
            else {
                for (auto &player : players) {
                    player.clear();
                    player.enableDrawing();
                    player.start();
                }
                warmUp = false;
            }
        }

        if (refreshTimer < refreshInterval)
            refreshTimer += dt;
        else {
            refreshTimer = 0;
            int blocked = 0;
            for (int id = 0; id < players.size(); id++) {
                if (turn[id]) {
                    if (toLeft[id])
                        players[id].turnLeft(refreshInterval);
                    else
                        players[id].turnRight(refreshInterval);
                }
                players[id].move(refreshInterval);
                sendPosition(id);
                if (players[id].isBlocked())
                    blocked++;
            }
            if (blocked >= players.size() - 1) {
                roundTimer += refreshInterval;
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
            if (roundTimer > ROUND_INTERVAL) {
                for (auto &player : players) {
                    player.reset();
                    player.disableDrawing();
                }
                sendNextRound();
                won = false;
                warmUp = true;
                startTimer = 0;
                roundTimer = 0;
                std::cout << "New game" << std::endl;
            }
        }
        dt = cl.restart().asSeconds();
    }
}

void GameServer::sendPosition(int playerId) {
    sendPacket.clear();
    sendPacket << playerId << players[playerId].getPosition().x << players[playerId].getPosition().y
               << players[playerId].isDrawing();
    for (auto client : clients)
        client->send(sendPacket);
}

void GameServer::sendStartGame() {
    sendPacket << ID_START_GAME;
    for (const auto &name : names)
        sendPacket << name;
    for (auto client : clients)
        client->send(sendPacket);
}

void GameServer::sendWinner(int wonId) {
    sendPacket.clear();
    sendPacket << ID_SHOW_WINNER << wonId;
    for (auto client : clients)
        client->send(sendPacket);
}

void GameServer::sendNextRound() {
    sendPacket.clear();
    sendPacket << ID_NEXT_ROUND;
    for (auto client : clients)
        client->send(sendPacket);
}

