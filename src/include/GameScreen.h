#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <chrono>
#include "Board.h"
#include "LeaderboardEntry.h"

class GameScreen {
public:
    GameScreen(unsigned int windowWidth, unsigned int windowHeight, int rows, int cols, int mines);
    void handleEvent(sf::RenderWindow &window, sf::Event &event);
    void update();
    void render(sf::RenderWindow &window);
    void startTimer();
    void pauseTimer();
    void resumeTimer();
    bool shouldOpenLeaderboard() const;
    void setPlayerName(const std::string &name);
    void resetLeaderboardFlag();

private:
    void togglePlayPause();
    void revealTile(int row, int col);
    void revealAdjacentTiles(int row, int col);
    void loseGame();
    void checkWinCondition();
    void resetGame();
    void drawBoard(sf::RenderWindow &window);
    void drawButtons(sf::RenderWindow &window);
    void drawCounter(sf::RenderWindow &window);
    void drawTimer(sf::RenderWindow &window);
    void addEntryToLeaderboard(int minutes, int seconds, const std::string &playerName);

    sf::Font font;
    sf::Text gameText;
    sf::RenderWindow window;
    unsigned int windowWidth;
    unsigned int windowHeight;
    int rows;
    int cols;
    int mines;
    Board board;
    bool isPlaying;
    bool gameLost;
    bool debugMode;
    bool gameWon;
    bool isPaused;
    int flagsPlaced;
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::duration<float> totalPausedDuration;
    std::chrono::time_point<std::chrono::high_resolution_clock> pauseStart;
    std::chrono::duration<float> pausedTime; 
    bool openLeaderboard;
    std::string playerName;
    sf::Texture hiddenTileTexture;
    sf::Texture revealedTileTexture;
    sf::Texture flagTexture;
    sf::Texture mineTexture;
    std::vector<sf::Texture> numberTextures;
    sf::Texture playButtonTexture;
    sf::Texture pauseButtonTexture;
    sf::Texture happyFaceTexture;
    sf::Texture sadFaceTexture;
    sf::Texture winFaceTexture;
    sf::Texture debugButtonTexture;
    sf::Texture leaderboardButtonTexture;
    sf::Texture digitsTexture;
    sf::Sprite playPauseButtonSprite;
    sf::Sprite faceSprite;
    sf::Sprite debugButtonSprite;
    sf::Sprite leaderboardButtonSprite;
    std::vector<std::vector<sf::Sprite>> boardSprites;
};

#endif
