#ifndef WELCOME_SCREEN_HPP
#define WELCOME_SCREEN_HPP

#include <SFML/Graphics.hpp>

class WelcomeScreen {
public:
    WelcomeScreen(unsigned int windowWidth, unsigned int windowHeight);
    void handleEvent(sf::RenderWindow &window, sf::Event &event);
    void update();
    void render(sf::RenderWindow &window);
    bool shouldClose() const;
private:
    void setTextProperties(sf::Text &text, float x, float y);
    void capitalizeFirstLetter(std::string &text);
    void updateUserTextPosition();
    void formatUserInput(std::string &text);

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