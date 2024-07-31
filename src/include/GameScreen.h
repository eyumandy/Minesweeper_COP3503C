#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H

#include <SFML/Graphics.hpp>
#include "Board.h"
#include <vector>
#include <chrono>

class GameScreen {
public:
    GameScreen(unsigned int windowWidth, unsigned int windowHeight, int rows, int cols, int mines);
    void handleEvent(sf::RenderWindow &window, sf::Event &event);
    void update();
    void render(sf::RenderWindow &window);
    void resetGame();

private:
    void drawBoard(sf::RenderWindow &window);
    void drawButtons(sf::RenderWindow &window);
    void drawCounter(sf::RenderWindow &window);
    void drawTimer(sf::RenderWindow &window);
    void revealTile(int row, int col);
    void revealAdjacentTiles(int row, int col);
    void loseGame();
    void checkWinCondition();

    sf::Text gameText;
    sf::Font font;
    sf::Texture hiddenTileTexture;
    sf::Texture revealedTileTexture;
    sf::Texture flagTexture;
    sf::Texture mineTexture;
    sf::Texture playButtonTexture;
    sf::Texture pauseButtonTexture;
    sf::Texture happyFaceTexture;
    sf::Texture sadFaceTexture;
    sf::Texture winFaceTexture;
    sf::Texture debugButtonTexture;
    sf::Texture leaderboardButtonTexture;
    sf::Texture digitsTexture;
    sf::Texture numberTextures[8];
    sf::Sprite playPauseButtonSprite;
    sf::Sprite faceSprite;
    sf::Sprite debugButtonSprite;
    sf::Sprite leaderboardButtonSprite;
    std::vector<std::vector<sf::Sprite>> boardSprites;
    bool isPlaying;
    bool gameLost;
    bool debugMode;
    bool gameWon;
    bool isPaused;
    int flagsPlaced;
    unsigned int windowWidth;
    unsigned int windowHeight;
    Board board;
    int rows;
    int cols;
    int mines;

    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> pauseStart;
    std::chrono::duration<float> totalPausedDuration;
    std::chrono::duration<float> pausedTime;
};

#endif