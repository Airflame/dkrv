#ifndef DKRV_TIMERBARS_H
#define DKRV_TIMERBARS_H
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include <vector>
#include <iostream>


class TimerBars {
private:
    std::vector<float> timers;
    sf::Vector2f position;
    sf::RectangleShape bar;
public:
    TimerBars();
    void clear();
    void push();
    void refresh(float dt);
    void setPosition(sf::Vector2f arg);
    void draw(sf::RenderWindow* window);
};


#endif //DKRV_TIMERBARS_H
