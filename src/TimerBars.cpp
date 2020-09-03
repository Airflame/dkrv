#include "../include/TimerBars.h"

TimerBars::TimerBars() {
    bar.setSize(sf::Vector2f(BAR_WIDTH, BAR_HEIGHT));
    bar.setFillColor(sf::Color(100, 100, 100, 200));
    bar.setOrigin(0, BAR_HEIGHT);
}

void TimerBars::clear() {
    timers.clear();
}

void TimerBars::push() {
    timers.push_back(EFFECT_TIMER);
}

void TimerBars::refresh(float dt) {
    for (float &timer : timers) {
        timer -= dt;
    }
    for (int i = 0; i < timers.size(); i++) {
        if (timers[i] <= 0)
            timers.erase(timers.begin() + i);
    }
}

void TimerBars::setPosition(sf::Vector2f arg) {
    position = arg;
}

void TimerBars::draw(sf::RenderWindow *window) {
    for (int i = 0; i < timers.size(); i++) {
        bar.setPosition(position.x - BAR_WIDTH / 2, position.y - BAR_HEIGHT * (2 * i + 1));
        bar.setSize(sf::Vector2f(BAR_WIDTH * timers[i] / EFFECT_TIMER, BAR_HEIGHT));
        window->draw(bar);
    }
}
