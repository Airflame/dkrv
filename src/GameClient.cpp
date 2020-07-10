#include "../include/GameClient.h"

GameClient::GameClient() {
    pointShape.setRadius(5);
    colors.push_back(sf::Color::Red);
    colors.push_back(sf::Color::Yellow);
    colors.push_back(sf::Color::Green);
    colors.push_back(sf::Color::Blue);
    for (int i = 0; i < colors.size(); i++)
        lines.emplace_back();
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
        if (id == -1)
            running = true;
        else {
            if (id >= 0 and id < colors.size() and running) {
                float x, y;
                packet >> x >> y;
                if (x < 0)
                    lines[id].clear();
                else
                    lines[id].push_back(sf::Vector2f(x, y));
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
            packet.clear();
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

        window->clear();
        draw();
        window->display();
        dt = cl.restart().asSeconds();
    }
}
