#include "../include/WinnerText.h"

WinnerText::WinnerText() {
    colors.emplace_back(214, 48, 49);
    colors.emplace_back(253, 203, 110);
    colors.emplace_back(39, 174, 96);
    colors.emplace_back(9, 132, 227);
    colors.emplace_back(232, 67, 147);
    background.setSize(sf::Vector2f(800, 100));
    background.setPosition(0, 200);
    text.setCharacterSize(80);
    text.setPosition(100, 210);
    if(!font.loadFromFile("Roboto-Regular.ttf"))
        std::cout << "ERROR: Cannot load font" << std::endl;
    text.setFont(font);
    textColor = sf::Color(236, 240, 241);
    text.setFillColor(textColor);
}

void WinnerText::draw(sf::RenderWindow *window) {
    window->draw(background);
    window->draw(text);
}

void WinnerText::setColor(int id) {
    if (id < 0)
        backgroundColor = sf::Color(127, 140, 141);
    else
        backgroundColor = colors[id];
    background.setFillColor(backgroundColor);
}

void WinnerText::setText(const std::string& name) {
    text.setString(name);
}

void WinnerText::animate(float percent) {
    if (percent < 0.2) {
        backgroundColor.a = percent / 0.2 * 200;
    }
    else if (percent > 0.8) {
        backgroundColor.a = (1 - percent) / 0.2 * 200;
    }
    else if (percent >= 0.2 and percent <= 0.8) {
        backgroundColor.a = 200;
    }
    else
        backgroundColor.a = 0;
    textColor.a = backgroundColor.a;
    background.setFillColor(backgroundColor);
    text.setFillColor(textColor);
}
