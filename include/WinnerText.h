#ifndef DKRV_WINNERTEXT_H
#define DKRV_WINNERTEXT_H
#include <iostream>
#include <SFML/Graphics.hpp>


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
    void setColor(int id);
    void setText(const std::string& name);
    void animate(float percent);
};


#endif //DKRV_WINNERTEXT_H