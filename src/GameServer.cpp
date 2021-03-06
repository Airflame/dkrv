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
                    playerTurns.push_back(false);
                    playerTurnsLeft.push_back(false);
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
                        playerTurns[i] = receivedTurn;
                        if (playerTurns[i]) {
                            packet >> receivedDirection;
                            playerTurnsLeft[i] = receivedDirection;
                        }
                        sendTurn(i, receivedTurn, receivedDirection);
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
    float dt = 0, startTimer = 0, roundTimer = 0, refreshTimer = 0, synchTimer = 0;
    const float refreshInterval = 1.0f / TPS;
    bool won = false, warmUp = true;
    int blocked = 0, wonId;

    sf::Thread thread(&GameServer::netLoop, this);
    thread.launch();

    for (auto &player : players) {
        player.clear();
        player.setAngle(0);
        player.setEnemies(&players);
    }
    sendPositions();

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
                for (int playerId = 0; playerId < players.size(); playerId++)
                    sendDrawing(playerId, true);
                warmUp = false;
            }
        }

        if (refreshTimer < refreshInterval)
            refreshTimer += dt;
        else {
            refreshTimer = 0;
            synchTimer += refreshInterval;
            if (synchTimer > SYNC_INTERVAL) {
                sendSynchronization();
                synchTimer = 0;
            }
            for (auto effect : effects)
                effect->evaluate(refreshInterval);
            if (rand() % EFFECT_FREQ == 0) {
                int effectType = rand() % 4;
                float x = rand() % WINDOW_SIZE;
                float y = rand() % WINDOW_SIZE;
                switch (effectType) {
                    case EFFECT_FAST_SELF:
                        effects.push_back(new EffectFast(x, y, true, players));
                        sendNewEffect(EFFECT_FAST_SELF, x, y);
                        break;
                    case EFFECT_FAST_OTHERS:
                        effects.push_back(new EffectFast(x, y, false, players));
                        sendNewEffect(EFFECT_FAST_OTHERS, x, y);
                        break;
                    case EFFECT_SLOW_SELF:
                        effects.push_back(new EffectSlow(x, y, true, players));
                        sendNewEffect(EFFECT_SLOW_SELF, x, y);
                        break;
                    case EFFECT_SLOW_OTHERS:
                        effects.push_back(new EffectSlow(x, y, false, players));
                        sendNewEffect(EFFECT_SLOW_OTHERS, x, y);
                        break;
                    default:
                        break;
                }
            }
            for (int id = 0; id < players.size(); id++) {
                bool enteredBlocked = players[id].isBlocked();
                if (playerTurns[id]) {
                    if (playerTurnsLeft[id])
                        players[id].turnLeft(refreshInterval);
                    else
                        players[id].turnRight(refreshInterval);
                }
                players[id].move(refreshInterval);
                int drawingAction = players[id].makeHoles(refreshInterval);
                if (drawingAction == DRAWING_DISABLED)
                    sendDrawing(id, false);
                if (drawingAction == DRAWING_ENABLED)
                    sendDrawing(id, true);
                for (int effectId = 0; effectId < effects.size(); effectId++) {
                    if (effects[effectId]->isCollected() and !effects[effectId]->sent) {
                        effects[effectId]->sent = true;
                        sendEffectCollected(effectId, effects[effectId]->getCollectedPlayerId());
                    }
                }
                if (players[id].isBlocked() != enteredBlocked) {
                    sendBlocked(id);
                    blocked++;
                }
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
                    player.resetModifiers();
                }
                sendNextRound();
                won = false;
                warmUp = true;
                blocked = 0;
                startTimer = roundTimer = 0;
                for (auto &&turn : playerTurns)
                    turn = false;
                for (auto effect : effects)
                    delete effect;
                effects.clear();
                sendPositions();
                std::cout << "New game" << std::endl;
            }
        }
        dt = cl.restart().asSeconds();
    }
}

void GameServer::sendPositions() {
    for (int playerId = 0; playerId < players.size(); playerId++) {
        sendPacket.clear();
        sendPacket << playerId << players[playerId].getPosition().x << players[playerId].getPosition().y
                   << players[playerId].isDrawing();
        for (auto client : clients)
            client->send(sendPacket);
    }
}

void GameServer::sendStartGame() {
    sendPacket.clear();
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

void GameServer::sendNewEffect(int effectType, float x, float y) {
    sendPacket.clear();
    sendPacket << ID_NEW_EFFECT << effectType << x << y;
    for (auto client : clients)
        client->send(sendPacket);
}

void GameServer::sendEffectCollected(int effectId, int playerId) {
    sendPacket.clear();
    sendPacket << ID_EFFECT_COLLECTED << effectId << playerId;
    for (auto client : clients)
        client->send(sendPacket);
}

void GameServer::sendTurn(int playerId, bool turn, bool turnLeft) {
    sendPacket.clear();
    sendPacket << ID_TURN << playerId << turn << turnLeft;
    for (auto client : clients)
        client->send(sendPacket);
}

void GameServer::sendDrawing(int playerId, bool drawing) {
    sendPacket.clear();
    sendPacket << ID_DRAWING << playerId << drawing;
    for (auto client : clients)
        client->send(sendPacket);
}

void GameServer::sendBlocked(int playerId) {
    sendPacket.clear();
    sendPacket << ID_BLOCKED << playerId;
    for (auto client : clients)
        client->send(sendPacket);
}

void GameServer::sendSynchronization() {
    for (int playerId = 0; playerId < players.size(); playerId++) {
        sendPacket.clear();
        sendPacket << ID_SYNCHRONIZE << playerId << players[playerId].getPosition().x
                   << players[playerId].getPosition().y << players[playerId].getAngle();
        for (auto client : clients)
            client->send(sendPacket);
    }
}

