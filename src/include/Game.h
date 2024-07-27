#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Config.h"
#include "WelcomeScreen.h"

class Game {
public:
    Game(const Config& config);
    void run();

private:
    void processEvents();
    void update();
    void render();

    sf::RenderWindow window;
    enum class Screen { Welcome /*, Game, Win */ };
    Screen currentScreen;

    WelcomeScreen welcomeScreen;
    // Other screen objects
};

#endif