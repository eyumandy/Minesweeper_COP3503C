#include <SFML/Graphics.hpp>
#include "include/WelcomeScreen.h"
#include "include/GameScreen.h"

int main() {
    unsigned int windowWidth = 800;
    unsigned int windowHeight = 600;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Minesweeper");
    WelcomeScreen welcomeScreen(windowWidth, windowHeight);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            welcomeScreen.handleEvent(window, event);
        }

        if (welcomeScreen.shouldClose()) {
            // Transition to the game screen
            GameScreen gameScreen(windowWidth, windowHeight); // Temporary GameScreen
            while (window.isOpen()) {
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }

                    gameScreen.handleEvent(window, event); // Handle game screen events
                }

                gameScreen.update(); // Update game screen logic
                window.clear();
                gameScreen.render(window); // Render game screen
                window.display();
            }
        }

        welcomeScreen.update();
        window.clear();
        welcomeScreen.render(window);
        window.display();
    }

    return 0;
}