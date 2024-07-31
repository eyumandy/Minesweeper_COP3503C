#include "include/LeaderboardScreen.h"
#include <iostream>

LeaderboardScreen::LeaderboardScreen(unsigned int windowWidth, unsigned int windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight),
      window(sf::VideoMode(windowWidth, windowHeight), "Leaderboard") {
    if (!font.loadFromFile("files/font.ttf")) {
        std::cerr << "Error loading font" << std::endl;
    }

    leaderboardText.setFont(font);
    leaderboardText.setString("Leaderboard");
    leaderboardText.setCharacterSize(24);
    leaderboardText.setFillColor(sf::Color::White);
    leaderboardText.setStyle(sf::Text::Bold);

    sf::FloatRect textRect = leaderboardText.getLocalBounds();
    leaderboardText.setOrigin(textRect.left + textRect.width / 2.0f,
                              textRect.top + textRect.height / 2.0f);
    leaderboardText.setPosition(windowWidth / 2.0f, 50);
}

void LeaderboardScreen::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            handleEvent(window, event);
        }
        update();
        render(window);
    }
}

void LeaderboardScreen::handleEvent(sf::RenderWindow &window, sf::Event &event) {
    if (event.type == sf::Event::Closed) {
        window.close();
    }
    // Add more event handling logic as needed
}

void LeaderboardScreen::update() {
    // Update logic if needed
}

void LeaderboardScreen::render(sf::RenderWindow &window) {
    window.clear(sf::Color::Blue); // Set background to blue
    window.draw(leaderboardText);
    window.display();
}
