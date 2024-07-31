#include "include/Config.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

Config::Config(const std::string& filePath) {
    loadConfig(filePath);
}

void Config::loadConfig(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open config file");
    }

    file >> columns >> rows >> mines;

    if (file.fail()) {
        throw std::runtime_error("Error reading config file");
    }

    std::cout << "Config loaded: " << columns << " columns, " << rows << " rows, " << mines << " mines" << std::endl;
}

int Config::getColumns() const {
    return columns;
}

int Config::getRows() const {
    return rows;
}

int Config::getMines() const {
    return mines;
}