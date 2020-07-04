#include "../include/Game.h"

Game::Game() {

}

void Game::netLoop() {

}

void Game::run() {
    sf::Clock cl;
    float dt = 0;
    Player player;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode(800, 800), "DKRV", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    //sf::Thread thread(&Game::netLoop, this);
    //thread.launch();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            player.turnLeft(dt);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            player.turnRight(dt);

        player.move(dt);

        window.clear();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            player.drawHead(&window);
        else
            player.drawLine(&window);
        window.display();
        dt = cl.restart().asSeconds();
    }
    //thread.terminate();
}
