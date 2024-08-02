#include "include/GameScreen.h"
#include "include/LeaderboardScreen.h"
#include "include/LeaderboardEntry.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>

GameScreen::GameScreen(unsigned int winWidth, unsigned int winHeight, int numRows, int numCols, int numMines)
    : windowWidth(winWidth), windowHeight(winHeight), rows(numRows), cols(numCols), mines(numMines),
      board(numRows, numCols, numMines), isPlaying(false), gameLost(false), debugMode(false), gameWon(false),
      isPaused(false), flagsPlaced(0), totalPausedDuration(0), pausedTime(std::chrono::duration<float>(0)), openLeaderboard(false) {

    font.loadFromFile("files/font.ttf");
    hiddenTileTexture.loadFromFile("files/images/tile_hidden.png");
    revealedTileTexture.loadFromFile("files/images/tile_revealed.png");
    flagTexture.loadFromFile("files/images/flag.png");
    mineTexture.loadFromFile("files/images/mine.png");

    numberTextures.resize(8);
    for (int i = 0; i < 8; ++i) {
        numberTextures[i].loadFromFile("files/images/number_" + std::to_string(i + 1) + ".png");
    }

    playButtonTexture.loadFromFile("files/images/play.png");
    pauseButtonTexture.loadFromFile("files/images/pause.png");
    happyFaceTexture.loadFromFile("files/images/face_happy.png");
    sadFaceTexture.loadFromFile("files/images/face_lose.png");
    winFaceTexture.loadFromFile("files/images/face_win.png");
    debugButtonTexture.loadFromFile("files/images/debug.png");
    leaderboardButtonTexture.loadFromFile("files/images/leaderboard.png");
    digitsTexture.loadFromFile("files/images/digits.png");


    gameText.setFont(font);
    gameText.setString("Game Screen");
    gameText.setCharacterSize(24);
    gameText.setFillColor(sf::Color::White);
    gameText.setStyle(sf::Text::Bold);

    sf::FloatRect textBounds = gameText.getLocalBounds();
    gameText.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    gameText.setPosition(windowWidth / 2.0f, 50);

    boardSprites.resize(rows, std::vector<sf::Sprite>(cols));
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            boardSprites[r][c].setTexture(hiddenTileTexture);
            boardSprites[r][c].setPosition(c * 32, r * 32);
        }
    }

    playPauseButtonSprite.setTexture(pauseButtonTexture);
    playPauseButtonSprite.setPosition((cols * 32) - 240, 32 * (rows + 0.5));

    faceSprite.setTexture(happyFaceTexture);
    faceSprite.setPosition((cols / 2.0 * 32) - 32, 32 * (rows + 0.5));

    debugButtonSprite.setTexture(debugButtonTexture);
    debugButtonSprite.setPosition((cols * 32) - 304, 32 * (rows + 0.5));

    leaderboardButtonSprite.setTexture(leaderboardButtonTexture);
    leaderboardButtonSprite.setPosition((cols * 32) - 176, 32 * (rows + 0.5));

    std::cout << "Initialized game screen with dimensions: " << windowWidth << "x" << windowHeight << std::endl;
}

void GameScreen::handleEvent(sf::RenderWindow &window, sf::Event &event) {
    if (event.type == sf::Event::Closed) {
        window.close();
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (playPauseButtonSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            std::cout << "Play/Pause button clicked" << std::endl;
            togglePlayPause();
        }
        if (faceSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            std::cout << "Face button clicked" << std::endl;
            resetGame();
        }
        if (!gameLost && !gameWon && debugButtonSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            std::cout << "Debug button clicked" << std::endl;
            debugMode = !debugMode;
        }
        if (leaderboardButtonSprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && !openLeaderboard) {
            std::cout << "Leaderboard button clicked" << std::endl;
            openLeaderboard = true;

            pauseTimer();
            playPauseButtonSprite.setTexture(playButtonTexture);

            int leaderboardWidth = (cols * 16);
            int leaderboardHeight = (rows * 16) + 50;
            LeaderboardScreen leaderboardScreen(leaderboardWidth, leaderboardHeight);
            leaderboardScreen.run();

            openLeaderboard = false;
        }

        if (!gameLost && !gameWon && !isPaused) {
            for (int r = 0; r < boardSprites.size(); ++r) {
                for (int c = 0; c < boardSprites[r].size(); ++c) {
                    sf::FloatRect tileBounds = boardSprites[r][c].getGlobalBounds();
                    if (tileBounds.contains(mousePos.x, mousePos.y)) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            revealTile(r, c);
                        } else if (event.mouseButton.button == sf::Mouse::Right) {
                            if (board.getTileState(r, c) == TileState::Hidden) {
                                board.setTileState(r, c, TileState::Flagged);
                                flagsPlaced++;
                            } else if (board.getTileState(r, c) == TileState::Flagged) {
                                board.setTileState(r, c, TileState::Hidden);
                                flagsPlaced--;
                            }
                            checkWinCondition();
                        }
                    }
                }
            }
        }
    }
}

void GameScreen::togglePlayPause() {
    isPlaying ? pauseTimer() : resumeTimer();
    playPauseButtonSprite.setTexture(isPlaying ? pauseButtonTexture : playButtonTexture);
}

void GameScreen::startTimer() {
    startTime = std::chrono::high_resolution_clock::now();
    totalPausedDuration = std::chrono::duration<float>(0);
    pausedTime = std::chrono::duration<float>(0);
    isPlaying = true;
    isPaused = false;
}

void GameScreen::pauseTimer() {
    if (!isPaused) {
        isPaused = true;
        isPlaying = false;
        pauseStart = std::chrono::high_resolution_clock::now();
    }
}

void GameScreen::resumeTimer() {
    if (isPaused) {
        isPaused = false;
        isPlaying = true;
        totalPausedDuration += std::chrono::high_resolution_clock::now() - pauseStart;
    }
}

void GameScreen::resetLeaderboardFlag() {
    openLeaderboard = false;
}

void GameScreen::revealTile(int row, int col) {
    if (board.getTileState(row, col) != TileState::Hidden) return;
    board.setTileState(row, col, TileState::Revealed);
    if (board.hasMine(row, col)) {
        loseGame();
    } else {
        int nearbyMines = board.getAdjacentMines(row, col);
        if (nearbyMines == 0) {
            revealAdjacentTiles(row, col);
        }
        checkWinCondition();
    }
}

void GameScreen::revealAdjacentTiles(int row, int col) {
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            int r = row + dr;
            int c = col + dc;
            if (r >= 0 && r < board.getBoard().size() && c >= 0 && c < board.getBoard()[r].size() &&
                board.getTileState(r, c) == TileState::Hidden) {
                revealTile(r, c);
            }
        }
    }
}

void GameScreen::loseGame() {
    gameLost = true;
    faceSprite.setTexture(sadFaceTexture);
    for (int r = 0; r < board.getBoard().size(); ++r) {
        for (int c = 0; c < board.getBoard()[r].size(); ++c) {
            if (board.hasMine(r, c)) {
                board.setTileState(r, c, TileState::Revealed);
            }
        }
    }
}

void GameScreen::checkWinCondition() {
    bool allMinesFlagged = true;
    bool allTilesRevealed = true;

    for (int r = 0; r < board.getBoard().size(); ++r) {
        for (int c = 0; c < board.getBoard()[r].size(); ++c) {
            if (board.hasMine(r, c)) {
                if (board.getTileState(r, c) != TileState::Flagged) {
                    allMinesFlagged = false;
                }
            } else {
                if (board.getTileState(r, c) != TileState::Revealed) {
                    allTilesRevealed = false;
                }
            }
        }
    }

    if (allMinesFlagged && allTilesRevealed) {
        gameWon = true;
        faceSprite.setTexture(winFaceTexture);


        isPlaying = false;
        playPauseButtonSprite.setTexture(playButtonTexture);

        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsedTime = currentTime - startTime - totalPausedDuration;
        int totalSeconds = static_cast<int>(elapsedTime.count());
        int minutes = totalSeconds / 60;
        int seconds = totalSeconds % 60;

        addEntryToLeaderboard(minutes, seconds, playerName);

        openLeaderboard = true;

        pauseTimer();
        playPauseButtonSprite.setTexture(playButtonTexture);

        int leaderboardWidth = (cols * 16);
        int leaderboardHeight = (rows * 16) + 50;
        LeaderboardScreen leaderboardScreen(leaderboardWidth, leaderboardHeight);
        leaderboardScreen.run();

        openLeaderboard = false;
    }
}


void GameScreen::addEntryToLeaderboard(int minutes, int seconds, const std::string& playerName) {
    std::vector<LeaderboardEntry> leaderboard;
    std::ifstream inputFile("files/leaderboard.txt");
    if (inputFile.is_open()) {
        std::string line;
        while (std::getline(inputFile, line)) {
            std::stringstream lineStream(line);
            std::string time, name;
            if (std::getline(lineStream, time, ',') && std::getline(lineStream, name)) {
                int min, sec;
                sscanf(time.c_str(), "%d:%d", &min, &sec);
                leaderboard.push_back({min, sec, name});
            }
        }
        inputFile.close();
    }

    leaderboard.push_back({minutes, seconds, playerName});

    std::sort(leaderboard.begin(), leaderboard.end(), [](const LeaderboardEntry &a, const LeaderboardEntry &b) {
        return a.totalSeconds() < b.totalSeconds();
    });

    std::ofstream outputFile("files/leaderboard.txt");
    if (outputFile.is_open()) {
        for (int i = 0; i < std::min(5, (int)leaderboard.size()); ++i) {
            outputFile << leaderboard[i].minutes << ":" << (leaderboard[i].seconds < 10 ? "0" : "")
                       << leaderboard[i].seconds << "," << leaderboard[i].playerName << std::endl;
        }
        outputFile.close();
    }

    std::cout << "New Leaderboard Entry: " << minutes << ":" << (seconds < 10 ? "0" : "") << seconds
              << "," << playerName << std::endl;
}

void GameScreen::resetGame() {
    gameLost = false;
    gameWon = false;
    isPlaying = true;
    isPaused = false;
    flagsPlaced = 0;
    totalPausedDuration = std::chrono::duration<float>(0);
    pausedTime = std::chrono::duration<float>(0);
    startTime = std::chrono::high_resolution_clock::now();
    faceSprite.setTexture(happyFaceTexture);
    playPauseButtonSprite.setTexture(pauseButtonTexture);
    board = Board(rows, cols, mines);
    openLeaderboard = false;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            boardSprites[r][c].setTexture(hiddenTileTexture);
            boardSprites[r][c].setPosition(c * 32, r * 32);
        }
    }
}

void GameScreen::update() {
    if (isPlaying && !isPaused) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsedTime = currentTime - startTime - totalPausedDuration;
    }
}



void GameScreen::render(sf::RenderWindow &window) {
    window.clear(sf::Color::White);
    window.draw(gameText);
    drawBoard(window);
    drawButtons(window);
    drawCounter(window);
    drawTimer(window);
}

void GameScreen::drawBoard(sf::RenderWindow &window) {
    for (int r = 0; r < boardSprites.size(); ++r) {
        for (int c = 0; c < boardSprites[r].size(); ++c) {
            if (isPaused && !gameWon) {
                boardSprites[r][c].setTexture(revealedTileTexture);
            } else {
                if (board.getTileState(r, c) == TileState::Hidden || board.getTileState(r, c) == TileState::Flagged) {
                    boardSprites[r][c].setTexture(hiddenTileTexture);
                } else if (board.getTileState(r, c) == TileState::Revealed) {
                    boardSprites[r][c].setTexture(revealedTileTexture);
                }
            }

            // Draw the base tile
            window.draw(boardSprites[r][c]);

            if (!isPaused || gameWon) {  // Normal behavior unless paused and game not won
                if (board.getTileState(r, c) == TileState::Revealed) {
                    if (board.hasMine(r, c)) {
                        sf::Sprite mineSprite(mineTexture);
                        mineSprite.setPosition(c * 32, r * 32);
                        window.draw(mineSprite);
                    } else {
                        int adjacentMines = board.getAdjacentMines(r, c);
                        if (adjacentMines > 0) {
                            sf::Sprite numberSprite(numberTextures[adjacentMines - 1]);
                            numberSprite.setPosition(c * 32, r * 32);
                            window.draw(numberSprite);
                        }
                    }
                } else if (board.getTileState(r, c) == TileState::Flagged) {
                    sf::Sprite flagSprite(flagTexture);
                    flagSprite.setPosition(c * 32, r * 32);
                    window.draw(flagSprite);
                }

                if (debugMode && board.hasMine(r, c) && board.getTileState(r, c) == TileState::Hidden) {
                    sf::Sprite mineSprite(mineTexture);
                    mineSprite.setPosition(c * 32, r * 32);
                    window.draw(mineSprite);
                }
            }
        }
    }
}

void GameScreen::drawButtons(sf::RenderWindow &window) {
    window.draw(playPauseButtonSprite);
    window.draw(faceSprite);
    window.draw(debugButtonSprite);
    window.draw(leaderboardButtonSprite);
}

void GameScreen::drawCounter(sf::RenderWindow &window) {
    int counterValue = mines - flagsPlaced;
    bool isNegative = counterValue < 0;
    counterValue = abs(counterValue);

    std::string counterStr = std::to_string(counterValue);
    if (counterStr.length() < 3) {
        counterStr.insert(0, 3 - counterStr.length(), '0');
    }

    if (isNegative) {
        sf::Sprite minusSprite(digitsTexture, sf::IntRect(210, 0, 21, 32));
        minusSprite.setPosition(12, 32 * (rows + 0.5) + 16);
        window.draw(minusSprite);
    }

    for (int i = 0; i < counterStr.length(); ++i) {
        int digit = counterStr[i] - '0';
        sf::Sprite digitSprite(digitsTexture, sf::IntRect(digit * 21, 0, 21, 32));
        digitSprite.setPosition(33 + i * 21, 32 * (rows + 0.5) + 16);
        window.draw(digitSprite);
    }
}

void GameScreen::drawTimer(sf::RenderWindow &window) {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsedTime;

    if (isPaused) {
        elapsedTime = pauseStart - startTime - totalPausedDuration;
    } else {
        elapsedTime = currentTime - startTime - totalPausedDuration;
    }

    int totalSeconds = static_cast<int>(elapsedTime.count());
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    std::string minuteStr = std::to_string(minutes);
    if (minuteStr.length() < 2) {
        minuteStr.insert(0, 2 - minuteStr.length(), '0');
    }

    std::string secondStr = std::to_string(seconds);
    if (secondStr.length() < 2) {
        secondStr.insert(0, 2 - secondStr.length(), '0');
    }

    int startXMinutes = (cols * 32) - 97;
    int startY = 32 * (rows + 0.5) + 16;

    for (int i = 0; i < minuteStr.length(); ++i) {
        int digit = minuteStr[i] - '0';
        sf::Sprite digitSprite(digitsTexture, sf::IntRect(digit * 21, 0, 21, 32));
        digitSprite.setPosition(startXMinutes + i * 21, startY);
        window.draw(digitSprite);
    }

    int startXSeconds = (cols * 32) - 54;

    for (int i = 0; i < secondStr.length(); ++i) {
        int digit = secondStr[i] - '0';
        sf::Sprite digitSprite(digitsTexture, sf::IntRect(digit * 21, 0, 21, 32));
        digitSprite.setPosition(startXSeconds + i * 21, startY);
        window.draw(digitSprite);
    }
}

bool GameScreen::shouldOpenLeaderboard() const {
    return openLeaderboard;
}

void GameScreen::setPlayerName(const std::string &name) {
    playerName = name;
}
