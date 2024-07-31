#ifndef GAME_SCREEN_HPP
#define GAME_SCREEN_HPP

#include <SFML/Graphics.hpp>

class GameScreen {
public:
    GameScreen(unsigned int windowWidth, unsigned int windowHeight);
    void handleEvent(sf::RenderWindow &window, sf::Event &event);
    void update();
    void render(sf::RenderWindow &window);

private:
    sf::Text gameText;
    sf::Font font;
    unsigned int windowWidth;
    unsigned int windowHeight;
};

#endif