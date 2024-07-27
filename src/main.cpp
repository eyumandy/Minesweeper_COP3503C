#include "include/Singleton.h"
#include <iostream>

int main() {
    try {
        Singleton::getInstance().initialize("files/config.cfg");
        Game* game = Singleton::getInstance().getGame();
        game->run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}