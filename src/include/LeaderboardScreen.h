#ifndef LEADERBOARD_SCREEN_H
#define LEADERBOARD_SCREEN_H

#include <SFML/Graphics.hpp>
#include "LeaderboardEntry.h"
#include <vector>

class LeaderboardScreen {
public:
    LeaderboardScreen(unsigned int windowWidth, unsigned int windowHeight);
    void run();
    void addEntry(const LeaderboardEntry &entry);
    void loadLeaderboard();
    void saveLeaderboard();

private:
    void handleEvent(sf::RenderWindow &window, sf::Event &event);
    void update();
    void render(sf::RenderWindow &window);
    
    sf::RenderWindow window;
    unsigned int windowWidth;
    unsigned int windowHeight;
    sf::Font font;
    sf::Text leaderboardText;
    std::vector<LeaderboardEntry> entries; // Make this private for better encapsulation
};

#endif
