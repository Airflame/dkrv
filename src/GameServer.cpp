#include "../include/GameServer.h"

GameServer::GameServer() {
    colors.push_back(sf::Color::Red);
    colors.push_back(sf::Color::Yellow);
    colors.push_back(sf::Color::Green);
    colors.push_back(sf::Color::Blue);
    listening = true;
    running = true;
}

void GameServer::listen() {
    int numberClients = 0, maxClients = 0;
    bool waiting = true;
    while (maxClients < 2 or maxClients > 4) {
        std::cout << "Number of players [2-4]:" << std::endl;
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
                    selector.add(*client);
                    Player player;
                    player.setColor(colors[numberClients]);
                    players.push_back(player);
                    std::cout << "New client (" << numberClients << ") - " << client->getRemotePort() << std::endl;
                    numberClients++;
                    if (numberClients >= maxClients)
                        selector.remove(listener);
                } else
                    delete client;
            } else {
                for (sf::TcpSocket *socket : clients) {
                    sf::TcpSocket &client = *socket;
                    if (selector.isReady(client)) {
                        sf::Packet packet;
                        if (client.receive(packet) == sf::Socket::Done) {
                            packet >> names[client.getRemotePort()];
                            std::cout << "Client " << client.getRemoteAddress()
                                      << " nickname - " << names[client.getRemotePort()] << std::endl;
                            if (names.size() >= maxClients)
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
        float dt;
        if (selector.wait()) {
            for (int i = 0; i < clients.size(); i++) {
                sf::TcpSocket &client = *clients[i];
                if (selector.isReady(client)) {
                    sf::Packet packet;
                    if (client.receive(packet) == sf::Socket::Done) {
                        packet >> toLeft >> dt;
                        if (toLeft)
                            players[i].turnLeft(dt);
                        else
                            players[i].turnRight(dt);
                    }
                }
            }
        }
    }
}

void GameServer::run() {
    listen();

    sf::Packet packet;
    packet << -1;
    for (auto client : clients)
        client->send(packet);

    srand(time(nullptr));
    sf::Clock cl;
    float dt = 0;
    float startInterval = 5;
    float refreshInterval = 1.0f / FPS;

    sf::Thread thread(&GameServer::netLoop, this);
    thread.launch();

    for (int i = 0; i < players.size(); i++) {
        players[i].clear();
        players[i].setEnemies(&players);
        players[i].enableDrawing();
    }

    while (running) {
//        if (startInterval > 0)
//            startInterval -= dt;
//        else {
//            for (auto player : players)
//                player.enableDrawing();
//        }

        if (refreshInterval > 0)
            refreshInterval -= dt;
        else {
            refreshInterval = 1.0f / FPS;
            for (int i = 0; i < players.size(); i++) {
                players[i].move(refreshInterval);
                packet.clear();
                packet << i << players[i].getPosition().x << players[i].getPosition().y;
                for (auto client : clients)
                    client->send(packet);
            }
        }

        dt = cl.restart().asSeconds();
    }
}

