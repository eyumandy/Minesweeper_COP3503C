#include "include/GameScreen.h"
#include <iostream>

GameScreen::GameScreen(unsigned int windowWidth, unsigned int windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight) {

    gameText.setFont(font);
    gameText.setString("Game Screen");
    gameText.setCharacterSize(24);
    gameText.setFillColor(sf::Color::Black);
    gameText.setStyle(sf::Text::Bold);

    // Set the position of the text
    sf::FloatRect textRect = gameText.getLocalBounds();
    gameText.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top + textRect.height / 2.0f);
    gameText.setPosition(windowWidth / 2.0f, windowHeight / 2.0f);
}

void GameScreen::handleEvent(sf::RenderWindow &window, sf::Event &event) {
    if (event.type == sf::Event::Closed) {
        window.close();
    }
}

void GameScreen::update() {
}

void GameScreen::render(sf::RenderWindow &window) {
    window.clear(sf::Color::White);
    window.draw(gameText);
}