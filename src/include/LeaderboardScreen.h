#ifndef LEADERBOARD_SCREEN_H
#define LEADERBOARD_SCREEN_H

#include <SFML/Graphics.hpp>

class LeaderboardScreen {
public:
    LeaderboardScreen(unsigned int windowWidth, unsigned int windowHeight);
    void run();
    void handleEvent(sf::RenderWindow &window, sf::Event &event);
    void update();
    void render(sf::RenderWindow &window);

private:
    unsigned int windowWidth;
    unsigned int windowHeight;
    sf::Font font;
    sf::Text leaderboardText;
    sf::RenderWindow window;
};

#endif
