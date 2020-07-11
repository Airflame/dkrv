#include "../include/GameClient.h"

GameClient::GameClient() {
    pointShape.setRadius(5);
    pointShape.setOrigin(5, 5);
    colors.emplace_back(214, 48, 49);
    colors.emplace_back(253, 203, 110);
    colors.emplace_back(39, 174, 96);
    colors.emplace_back(9, 132, 227);
    colors.emplace_back(232, 67, 147);
    for (int i = 0; i < colors.size(); i++) {
        lines.emplace_back();
        positions.emplace_back(sf::Vector2f(-10, -10));
    }
    listening = true;
    running = false;
}

void GameClient::connect() {
    std::cout << "Enter IP address:" << std::endl;
    std::string strIp;
    std::cin >> strIp;
    sf::IpAddress ip(strIp);
    server.connect(ip, 7777);
    std::cout << "Connection established" << std::endl;

    std::string name;
    std::cout << std::endl << "Enter name:" << std::endl;
    std::cin >> name;
    sf::Packet packet;
    packet << name;
    server.send(packet);
}

void GameClient::netLoop() {
    while (listening) {
        int id;
        sf::Packet packet;
        server.receive(packet);
        packet >> id;
        if (id == -1) {
            running = true;
            while (!packet.endOfPacket()) {
                std::string name;
                packet >> name;
                names.push_back(name);
            }
        } else if (id == -2) {
            for (auto &line : lines)
                line.clear();
        } else if (id == -3) {
            int winId;
            packet >> winId;
            if (winId < 0)
                std::cout << "Draw" << std::endl;
            else
                std::cout << "Winner: " << names[winId] << std::endl;
        }
        else {
            if (id >= 0 and id < colors.size() and running) {
                float x, y;
                bool draw;
                packet >> x >> y >> draw;
                if (x < 0)
                    lines[id].clear();
                else {
                    positions[id] = sf::Vector2f(x, y);
                    if (draw)
                        lines[id].push_back(sf::Vector2f(x, y));
                }
            }
        }
    }
}

void GameClient::draw() {
    for (int i = 0; i < lines.size(); i++) {
        pointShape.setFillColor(colors[i]);
        for (auto position : lines[i]) {
            pointShape.setPosition(position);
            window->draw(pointShape);
        }
        pointShape.setPosition(positions[i]);
        window->draw(pointShape);
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

    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    window = new sf::RenderWindow(sf::VideoMode(800, 800), "DKRV", sf::Style::Default, settings);
    window->setFramerateLimit(60);
    sf::Packet packet;

    while (window->isOpen()) {
        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                thread.terminate();
                window->close();
                delete window;
                return;
            }
        }

        if (window->hasFocus()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                packet.clear();
                packet << true << dt;
                server.send(packet);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                packet.clear();
                packet << false << dt;
                server.send(packet);
            }
        }

        window->clear(sf::Color(30, 39, 46));
        draw();
        window->display();
        dt = cl.restart().asSeconds();
    }
}
