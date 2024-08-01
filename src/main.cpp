#include <SFML/Graphics.hpp>
#include <iostream>
#include "include/Config.h"
#include "include/WelcomeScreen.h"
#include "include/GameScreen.h"
#include "include/LeaderboardScreen.h"

class Game {
public:
    Game(const Config& config)
        : window(sf::VideoMode(config.getColumns() * 32, (config.getRows() * 32) + 100), "Minesweeper"),
          welcomeScreen(config.getColumns() * 32, (config.getRows() * 32) + 100),
          gameScreen(config.getColumns() * 32, (config.getRows() * 32) + 100, config.getRows(), config.getColumns(), config.getMines()),
          currentScreen(Screen::Welcome),
          config(config) {
        sf::Vector2u windowSize = window.getSize();
        std::cout << "Main window size: " << windowSize.x << "x" << windowSize.y << std::endl;
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
                        gameScreen.startTimer(); // Start timer when entering game screen
                    }
                    break;
                case Screen::Game:
                    gameScreen.handleEvent(window, event);
                    if (gameScreen.shouldOpenLeaderboard()) {
                        openLeaderboard(); // Open leaderboard window
                    }
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

    void openLeaderboard() {
        unsigned int leaderboardWidth = (config.getColumns() * 16);
        unsigned int leaderboardHeight = (config.getRows() * 16) + 50;

        gameScreen.pauseTimer(); // Pause the timer when entering leaderboard

        LeaderboardScreen leaderboardScreen(leaderboardWidth, leaderboardHeight);
        leaderboardScreen.run();

        // After closing the leaderboard, keep the timer paused
        gameScreen.resetLeaderboardFlag();
    }

    sf::RenderWindow window;
    Screen currentScreen;
    WelcomeScreen welcomeScreen;
    GameScreen gameScreen;
    Config config;
};

int main() {
    Config config("files/config.cfg");
    Game game(config);
    game.run();
    return 0;
}
