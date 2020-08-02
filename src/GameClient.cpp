#include "../include/GameClient.h"

GameClient::GameClient() {
    colors.emplace_back(214, 48, 49);
    colors.emplace_back(253, 203, 110);
    colors.emplace_back(39, 174, 96);
    colors.emplace_back(9, 132, 227);
    colors.emplace_back(232, 67, 147);
    for (int i = 0; i < 2; i++)
        textures.emplace_back();
    textures[0].loadFromFile("img/fast.png");
    textures[1].loadFromFile("img/slow.png");
    listening = true;
    running = false;
    drawWinnerText = false;
}

void GameClient::connect() {
    bool connected = false;
    while (!connected) {
        std::cout << "Enter IP address:" << std::endl;
        std::string strIp;
        std::cin >> strIp;
        sf::IpAddress ip(strIp);
        if (server.connect(ip, 7777, sf::seconds(5)) == sf::Socket::Done) {
            std::cout << "Connection established" << std::endl;
            connected = true;
        } else
            std::cout << "Connection failed" << std::endl << std::endl;
    }
    std::string name;
    std::cout << std::endl << "Enter name:" << std::endl;
    std::cin >> name;
    sf::Packet packet;
    packet << name;
    server.send(packet);
}

void GameClient::netLoop() {
    int id;
    sf::Packet packet;
    while (listening) {
        server.receive(packet);
        packet >> id;
        if (id == ID_START_GAME) {
            running = true;
            while (!packet.endOfPacket()) {
                std::string name;
                packet >> name;
                names.push_back(name);
                players.emplace_back();
            }
            for (int i = 0; i < players.size(); i++) {
                players[i].setColor(colors[i]);
            }
        } else if (id == ID_NEXT_ROUND) {
            for (auto &player : players)
                player.clear();
            drawWinnerText = false;
            for (auto effect : effects)
                delete effect;
            effects.clear();
        } else if (id == ID_SHOW_WINNER) {
            int winnerId;
            packet >> winnerId;
            if (winnerId == DRAW) {
                std::cout << "Draw" << std::endl;
                winnerText.setText("Draw");
            } else {
                std::cout << "Winner: " << names[winnerId] << std::endl;
                winnerText.setText(names[winnerId]);
            }
            winnerText.setColor(winnerId);
            drawWinnerText = true;
        } else if (id == ID_NEW_EFFECT) {
            int effectType;
            float x, y;
            packet >> effectType >> x >> y;
            switch (effectType) {
                case EFFECT_FAST_SELF:
                    effects.push_back(new EffectFast(x, y, true, players));
                    effects[effects.size() - 1]->setTexture(&textures[0]);
                    break;
                case EFFECT_FAST_OTHERS:
                    effects.push_back(new EffectFast(x, y, false, players));
                    effects[effects.size() - 1]->setTexture(&textures[0]);
                    break;
                case EFFECT_SLOW_SELF:
                    effects.push_back(new EffectSlow(x, y, true, players));
                    effects[effects.size() - 1]->setTexture(&textures[1]);
                    break;
                case EFFECT_SLOW_OTHERS:
                    effects.push_back(new EffectSlow(x, y, false, players));
                    effects[effects.size() - 1]->setTexture(&textures[1]);
                    break;
                default:
                    break;
            }
        } else if (id == ID_EFFECT_COLLECTED) {
            int effectId;
            packet >> effectId;
            effects[effectId]->finish();
        } else {
            if (id >= 0 and id < players.size() and running) {
                float x, y;
                bool draw;
                packet >> x >> y >> draw;
                sf::Vector2f receivedPosition(x, y);
                players[id].setPosition(receivedPosition);
                if (draw)
                    players[id].addPosition(receivedPosition);
            }
        }
    }
}

void GameClient::run() {
    connect();
    sf::Thread thread(&GameClient::netLoop, this);
    thread.launch();

    std::cout << "Waiting for server..." << std::endl;
    while (!running) {}
    std::cout << "Opening window..." << std::endl;

    srand(time(nullptr));
    sf::Clock cl;
    float dt = 0;
    float winnerTextTimer = 0;
    bool turnsLeft = false, turnsRight = false;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "DKRV", sf::Style::Default, settings);
    window->setFramerateLimit(FPS);
    window->setKeyRepeatEnabled(false);
    sf::Packet packet;
    for (auto& player : players)
        player.clear();

    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::KeyReleased and
                (event.key.code == sf::Keyboard::Left or event.key.code == sf::Keyboard::Right or
                 event.key.code == sf::Keyboard::A or event.key.code == sf::Keyboard::D) or
                event.type == sf::Event::LostFocus) {
                packet.clear();
                packet << false;
                server.send(packet);
                turnsLeft = turnsRight = false;
            }
            if (event.type == sf::Event::Closed) {
                thread.terminate();
                window->close();
                delete window;
                for (auto effect : effects)
                    delete effect;
                effects.clear();
                return;
            }
        }

        if (window->hasFocus()) {
            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) or sf::Keyboard::isKeyPressed(sf::Keyboard::A)) and
                !turnsLeft) {
                packet.clear();
                packet << true << TURN_LEFT;
                server.send(packet);
                turnsLeft = true;
            }
            if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) or sf::Keyboard::isKeyPressed(sf::Keyboard::D)) and
                !turnsRight) {
                packet.clear();
                packet << true << TURN_RIGHT;
                server.send(packet);
                turnsRight = true;
            }
        }

        window->clear(sf::Color(30, 39, 46));
        for (auto effect : effects)
            effect->draw(window);
        for (auto player : players)
            player.draw(window);
        if (drawWinnerText) {
            winnerText.animate(winnerTextTimer / WINNERTEXT_INTERVAL);
            winnerText.draw(window);
            winnerTextTimer += dt;
        } else
            winnerTextTimer = 0;
        window->display();
        dt = cl.restart().asSeconds();
    }
}
