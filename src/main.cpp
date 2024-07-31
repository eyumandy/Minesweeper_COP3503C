#include <SFML/Graphics.hpp>
#include <iostream>
#include "include/Config.h"
#include "include/WelcomeScreen.h"
#include "include/GameScreen.h"

class Game {
public:
    Game(const Config& config)
        : window(sf::VideoMode(config.getColumns() * 32, (config.getRows() * 32) + 100), "Minesweeper"),
          welcomeScreen(config.getColumns() * 32, (config.getRows() * 32) + 100),
          gameScreen(config.getColumns() * 32, (config.getRows() * 32) + 100, config.getRows(), config.getColumns(), config.getMines()),
          currentScreen(Screen::Welcome) {
        // Print window dimensions to ensure correct creation
        sf::Vector2u windowSize = window.getSize();
        std::cout << "Window size: " << windowSize.x << "x" << windowSize.y << std::endl;
    }

    void run() {
        while (window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }

private:
    enum class Screen { Welcome, Game };

    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            switch (currentScreen) {
                case Screen::Welcome:
                    welcomeScreen.handleEvent(window, event);
                    if (welcomeScreen.shouldClose()) {
                        currentScreen = Screen::Game;
                    }
                    break;
                case Screen::Game:
                    gameScreen.handleEvent(window, event);
                    break;
            }
        }
    }

    void update() {
        switch (currentScreen) {
            case Screen::Welcome:
                welcomeScreen.update();
                break;
            case Screen::Game:
                gameScreen.update();
                break;
        }
    }

    void render() {
        window.clear(sf::Color::Black);

        switch (currentScreen) {
            case Screen::Welcome:
                welcomeScreen.render(window);
                break;
            case Screen::Game:
                gameScreen.render(window);
                break;
        }

        window.display();
    }

    sf::RenderWindow window;
    Screen currentScreen;
    WelcomeScreen welcomeScreen;
    GameScreen gameScreen;
};

int main() {
    Config config("files/config.cfg");
    Game game(config);
    game.run();
    return 0;
}