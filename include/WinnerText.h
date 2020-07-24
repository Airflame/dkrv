#ifndef DKRV_WINNERTEXT_H
#define DKRV_WINNERTEXT_H
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Constants.h"


class WinnerText {
private:
    sf::RectangleShape background;
    sf::Text text;
    sf::Color backgroundColor, textColor;
    sf::Font font;
    std::vector<sf::Color> colors;

public:
    WinnerText();
    void draw(sf::RenderWindow* window);
    void setColor(int playerId);
    void setText(const std::string& name);
    void animate(float percent);
};


#endif //DKRV_WINNERTEXT_H
