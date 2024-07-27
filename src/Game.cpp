#include "include/Game.h"

Game::Game(const Config& config)
    : window(sf::VideoMode(config.getRows() * 32, (config.getColumns() * 32) + 100), "Minesweeper"),
      currentScreen(Screen::Welcome),
      welcomeScreen(window.getSize().x, window.getSize().y) {}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        switch (currentScreen) {
            case Screen::Welcome:
                welcomeScreen.handleEvent(window, event);
                break;
            // Additional cases for other screens
        }
    }
}

void Game::update() {
    switch (currentScreen) {
        case Screen::Welcome:
            welcomeScreen.update();
            break;
        // Additional cases for other screens
    }
}

void Game::render() {
    window.clear(); // Clear window with default color (black)

    switch (currentScreen) {
        case Screen::Welcome:
            welcomeScreen.render(window);
            break;
        // Additional cases for other screens
    }

    window.display(); // Display rendered frame
}