#include "../include/Game.h"

Game::Game() {
    isListening = true;
    player.setEnemy(&enemy);
    enemy.setColor(sf::Color::Red);
}

void Game::listen() {
    std::cout << "Waiting for connection" << std::endl;
    sf::TcpListener listener;
    listener.listen(7777);
    listener.accept(socket);
    std::cout << "Connection established" << std::endl;
}

void Game::connect() {
    std::cout << "Enter IP address:" << std::endl;
    std::string strIp;
    std::cin >> strIp;
    sf::IpAddress ip(strIp);
    socket.connect(ip, 7777);
    std::cout << "Connection established" << std::endl;
}

void Game::netLoop() {
    while (isListening) {
        float x, y;
        sf::Packet packet;
        socket.receive(packet);
        packet >> x >> y;
        enemy.addPosition(sf::Vector2f(x, y));
    }
}

void Game::run(bool isServer) {
    srand(time(nullptr));
    sf::Clock cl;
    float dt = 0;
    float startInterval = 5;

    if (isServer)
        listen();
    else
        connect();

    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode(800, 800), "DKRV", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    sf::Thread thread(&Game::netLoop, this);
    thread.launch();
    sf::Packet packet;

    player.clear();
    enemy.clear();

    while (window.isOpen())
    {
        if (startInterval > 0)
            startInterval -= dt;
        else {
            player.enableDrawing();
            enemy.enableDrawing();
        }
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                isListening = false;
                thread.terminate();
                window.close();
                return;
            }
        }

        if (window.hasFocus()) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                player.turnLeft(dt);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                player.turnRight(dt);
        }

        player.move(dt);
        packet.clear();
        packet << player.getPosition().x << player.getPosition().y;
        socket.send(packet);

        window.clear();
        player.drawLine(&window);
        enemy.drawLine(&window);
        window.display();
        dt = cl.restart().asSeconds();
    }
}
