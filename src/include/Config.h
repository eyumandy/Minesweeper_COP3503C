#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config {
public:
    Config(const std::string& filePath);
    int getColumns() const;
    int getRows() const;
    int getMines() const;

private:
    void loadConfig(const std::string& filePath);

    int columns;
    int rows;
    int mines;
};

#endif