#include "include/WelcomeScreen.h"
#include <iostream>
#include <cctype>

WelcomeScreen::WelcomeScreen(unsigned int windowWidth, unsigned int windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight), closeFlag(false) {
    if (!font.loadFromFile("files/font.ttf")) {
        std::cerr << "Error loading font" << std::endl;
        return;
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

    userText.setFont(font);
    userText.setCharacterSize(18);
    userText.setFillColor(sf::Color::Yellow);

    setTextProperties(welcomeText, windowWidth / 2.0f, (windowHeight / 2.0f) - 150);
    setTextProperties(promptText, windowWidth / 2.0f, (windowHeight / 2.0f) - 75);
    updateUserTextPosition();
}

void WelcomeScreen::setTextProperties(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
    text.setPosition(x, y);
}

void WelcomeScreen::formatUserInput(std::string &text) {
    if (!text.empty()) {
        text[0] = std::toupper(text[0]);
        for (size_t i = 1; i < text.length(); ++i) {
            text[i] = std::tolower(text[i]);
        }
    }
}

void WelcomeScreen::updateUserTextPosition() {
    userText.setString(userInput + "|");
    setTextProperties(userText, windowWidth / 2.0f, (windowHeight / 2.0f) - 45);
}

void WelcomeScreen::handleEvent(sf::RenderWindow &window, sf::Event &event) {
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode < 128) {
            char enteredChar = static_cast<char>(event.text.unicode);
            if (enteredChar == '\b') {
                if (!userInput.empty()) {
                    userInput.pop_back();
                }
            } else if (std::isalpha(enteredChar)) {
                if (userInput.size() < 10) {
                    userInput += enteredChar;
                    formatUserInput(userInput);
                }
            }
            updateUserTextPosition();
        }
    } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
        if (!userInput.empty()) {
            closeFlag = true;
        }
    }
}

bool WelcomeScreen::shouldClose() const {
    return closeFlag;
}

std::string WelcomeScreen::getPlayerName() const {
    return userInput;
}

void WelcomeScreen::render(sf::RenderWindow &window) {
    window.clear(sf::Color::Blue);  // Use a background color of your choice

    // Draw text elements
    window.draw(welcomeText);
    window.draw(promptText);
    window.draw(userText);

    window.display();
}

void WelcomeScreen::update() {
    // Any animations or updates specific to the welcome screen can go here
    // For example, blinking cursor or animations
}
