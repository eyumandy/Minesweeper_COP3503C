#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include <SFML/Graphics.hpp>
#include <string>

class WelcomeScreen {
public:
    WelcomeScreen(unsigned int windowWidth, unsigned int windowHeight);
    void handleEvent(sf::RenderWindow &window, sf::Event &event);
    bool shouldClose() const;
    std::string getPlayerName() const;

    void render(sf::RenderWindow &window);
    void update();

private:
    void setTextProperties(sf::Text &text, float x, float y);
    void formatUserInput(std::string &text);
    void updateUserTextPosition();

    sf::Font font;
    sf::Text welcomeText;
    sf::Text promptText;
    sf::Text userText;
    unsigned int windowWidth;
    unsigned int windowHeight;
    bool closeFlag;
    std::string userInput;
};

#endif
