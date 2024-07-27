#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include "Config.h"
#include "Game.h"
#include "WelcomeScreen.h"

class Singleton {
public:
    static Singleton& getInstance();

    Config* getConfig() const;
    Game* getGame() const;
    WelcomeScreen* getWelcomeScreen() const;

    void initialize(const std::string& configFilePath);

private:
    Singleton();
    ~Singleton();
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    Config* config;
    Game* game;
    WelcomeScreen* welcomeScreen;
};

#endif