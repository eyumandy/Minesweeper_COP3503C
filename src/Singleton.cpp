#include "include/Singleton.h"
#include <stdexcept>

Singleton::Singleton() : config(nullptr), game(nullptr), welcomeScreen(nullptr) {}

Singleton::~Singleton() {
    delete config;
    delete game;
    delete welcomeScreen;
}

Singleton& Singleton::getInstance() {
    static Singleton instance;
    return instance;
}

void Singleton::initialize(const std::string& configFilePath) {
    config = new Config(configFilePath);
    game = new Game(*config);
}

Config* Singleton::getConfig() const {
    return config;
}

Game* Singleton::getGame() const {
    return game;
}

WelcomeScreen* Singleton::getWelcomeScreen() const {
    return welcomeScreen;
}