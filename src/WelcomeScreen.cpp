#include "include/WelcomeScreen.h"
#include <iostream>

WelcomeScreen::WelcomeScreen(unsigned int windowWidth, unsigned int windowHeight) {
    if (!font.loadFromFile("files/font.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        // Handle error, e.g., throw an exception or exit
    }
    welcomeText.setFont(font);
    welcomeText.setString("WELCOME TO MINESWEEPER!");
    welcomeText.setCharacterSize(24);
    welcomeText.setFillColor(sf::Color::White);
    welcomeText.setStyle(sf::Text::Underlined);

    promptText.setFont(font);
    promptText.setString("Enter your name:");
    promptText.setCharacterSize(20);
    promptText.setFillColor(sf::Color::White);

    // Set the position of the text using the helper function
    setTextProperties(welcomeText, windowWidth / 2.0f, (windowHeight / 2.0f) - 150);
    setTextProperties(promptText, windowWidth / 2.0f, (windowHeight / 2.0f) - 75);
}

void WelcomeScreen::setTextProperties(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
    text.setPosition(x, y);
}

void WelcomeScreen::handleEvent(sf::RenderWindow &window, sf::Event &event) {
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        // Logic to transition to the next screen
    }
}

void WelcomeScreen::update() {
    // Update logic if needed
}

void WelcomeScreen::render(sf::RenderWindow &window) {
    window.clear(sf::Color::Blue); // Set background to blue
    window.draw(welcomeText);
    window.draw(promptText);
}