#ifndef WELCOME_SCREEN_H
#define WELCOME_SCREEN_H

#include <SFML/Graphics.hpp>
#include <string>

class WelcomeScreen {
public:
    WelcomeScreen(unsigned int windowWidth, unsigned int windowHeight);
    void handleEvent(sf::RenderWindow &window, sf::Event &event);
    void update();
    void render(sf::RenderWindow &window);
    bool shouldClose() const;

private:
    void setTextProperties(sf::Text &text, float x, float y);
    void formatUserInput(std::string &text);
    void updateUserTextPosition();

    sf::Text welcomeText;
    sf::Text promptText;
    sf::Text userText;
    sf::Font font;
    std::string userInput;
    unsigned int windowWidth;
    unsigned int windowHeight;
    bool closeFlag;
};

#endif