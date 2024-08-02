#include "include/GameScreen.h"
#include "include/LeaderboardScreen.h"
#include "include/LeaderboardEntry.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>

GameScreen::GameScreen(unsigned int windowWidth, unsigned int windowHeight, int rows, int cols, int mines)
    : windowWidth(windowWidth), windowHeight(windowHeight), rows(rows), cols(cols), mines(mines), board(rows, cols, mines), isPlaying(false), gameLost(false), debugMode(false), gameWon(false), isPaused(false), flagsPlaced(0), totalPausedDuration(0), pausedTime(std::chrono::duration<float>(0)), openLeaderboard(false) {
    if (!font.loadFromFile("files/font.ttf")) {
        std::cerr << "Error loading font" << std::endl;
    }

    if (!hiddenTileTexture.loadFromFile("files/images/tile_hidden.png")) {
        std::cerr << "Error loading tile_hidden.png" << std::endl;
    }

    if (!revealedTileTexture.loadFromFile("files/images/tile_revealed.png")) {
        std::cerr << "Error loading tile_revealed.png" << std::endl;
    }

    if (!flagTexture.loadFromFile("files/images/flag.png")) {
        std::cerr << "Error loading flag.png" << std::endl;
    }

    if (!mineTexture.loadFromFile("files/images/mine.png")) {
        std::cerr << "Error loading mine.png" << std::endl;
    }

    numberTextures.resize(8); // Ensure the vector is properly sized
    for (int i = 0; i < 8; ++i) {
        if (!numberTextures[i].loadFromFile("files/images/number_" + std::to_string(i + 1) + ".png")) {
            std::cerr << "Error loading number_" << (i + 1) << ".png" << std::endl;
        }
    }

    if (!playButtonTexture.loadFromFile("files/images/play.png")) {
        std::cerr << "Error loading play.png" << std::endl;
    }

    if (!pauseButtonTexture.loadFromFile("files/images/pause.png")) {
        std::cerr << "Error loading pause.png" << std::endl;
    }

    if (!happyFaceTexture.loadFromFile("files/images/face_happy.png")) {
        std::cerr << "Error loading face_happy.png" << std::endl;
    }

    if (!sadFaceTexture.loadFromFile("files/images/face_lose.png")) {
        std::cerr << "Error loading face_lose.png" << std::endl;
    }

    if (!winFaceTexture.loadFromFile("files/images/face_win.png")) {
        std::cerr << "Error loading face_win.png" << std::endl;
    }

    if (!debugButtonTexture.loadFromFile("files/images/debug.png")) {
        std::cerr << "Error loading debug.png" << std::endl;
    }

    if (!leaderboardButtonTexture.loadFromFile("files/images/leaderboard.png")) {
        std::cerr << "Error loading leaderboard.png" << std::endl;
    }

    if (!digitsTexture.loadFromFile("files/images/digits.png")) {
        std::cerr << "Error loading digits.png" << std::endl;
    }

    gameText.setFont(font);
    gameText.setString("Game Screen");
    gameText.setCharacterSize(24);
    gameText.setFillColor(sf::Color::White);
    gameText.setStyle(sf::Text::Bold);

    sf::FloatRect textRect = gameText.getLocalBounds();
    gameText.setOrigin(textRect.left + textRect.width / 2.0f,
                       textRect.top + textRect.height / 2.0f);
    gameText.setPosition(windowWidth / 2.0f, 50);

    // Initialize boardSprites with hidden tiles
    boardSprites.resize(rows, std::vector<sf::Sprite>(cols));
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            boardSprites[r][c].setTexture(hiddenTileTexture);
            boardSprites[r][c].setPosition(c * 32, r * 32); // Offset for game text
        }
    }

    // Initialize button sprites
    playPauseButtonSprite.setTexture(pauseButtonTexture);
    playPauseButtonSprite.setPosition((cols * 32) - 240, 32 * (rows + 0.5));

    faceSprite.setTexture(happyFaceTexture);
    faceSprite.setPosition((cols / 2.0 * 32) - 32, 32 * (rows + 0.5));

    debugButtonSprite.setTexture(debugButtonTexture);
    debugButtonSprite.setPosition((cols * 32) - 304, 32 * (rows + 0.5));

    leaderboardButtonSprite.setTexture(leaderboardButtonTexture);
    leaderboardButtonSprite.setPosition((cols * 32) - 176, 32 * (rows + 0.5));

    std::cout << "Game screen size: " << windowWidth << "x" << windowHeight << std::endl;
}

void GameScreen::handleEvent(sf::RenderWindow &window, sf::Event &event) {
    if (event.type == sf::Event::Closed) {
        window.close();
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Handle mouse clicks on the buttons
        if (playPauseButtonSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            std::cout << "Clicked on Play/Pause button" << std::endl;
            togglePlayPause();
        }
        if (faceSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            std::cout << "Clicked on Face button" << std::endl;
            resetGame();
        }
        if (!gameLost && !gameWon && debugButtonSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            std::cout << "Clicked on Debug button" << std::endl;
            debugMode = !debugMode;
        }
        if (leaderboardButtonSprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && !openLeaderboard) {
            std::cout << "Clicked on Leaderboard button" << std::endl;
            openLeaderboard = true; // Set flag to open leaderboard

            // Pause the game and update the play/pause button
            pauseTimer();
            playPauseButtonSprite.setTexture(playButtonTexture);

            // Calculate leaderboard dimensions and open leaderboard window
            int leaderboardWidth = (cols * 16);
            int leaderboardHeight = (rows * 16) + 50;
            LeaderboardScreen leaderboardScreen(leaderboardWidth, leaderboardHeight);
            leaderboardScreen.run();

            // Reset the openLeaderboard flag after the leaderboard is shown
            openLeaderboard = false;
        }

        // Handle mouse clicks on the board
        if (!gameLost && !gameWon && !isPaused) {
            for (int r = 0; r < boardSprites.size(); ++r) {
                for (int c = 0; c < boardSprites[r].size(); ++c) {
                    sf::FloatRect bounds = boardSprites[r][c].getGlobalBounds();
                    if (bounds.contains(mousePos.x, mousePos.y)) {
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
    if (isPlaying) {
        pauseTimer();
        playPauseButtonSprite.setTexture(playButtonTexture);
    } else {
        resumeTimer();
        playPauseButtonSprite.setTexture(pauseButtonTexture);
    }
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
        int adjacentMines = board.getAdjacentMines(row, col);
        if (adjacentMines == 0) {
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
    // Add additional logic to end the game
}

void GameScreen::checkWinCondition() {
    bool allBombsFlagged = true;
    bool allNonBombsRevealed = true;

    for (int r = 0; r < board.getBoard().size(); ++r) {
        for (int c = 0; c < board.getBoard()[r].size(); ++c) {
            if (board.hasMine(r, c)) {
                // Check if every bomb is flagged
                if (board.getTileState(r, c) != TileState::Flagged) {
                    allBombsFlagged = false;
                }
            } else {
                // Check if all non-bomb tiles are revealed
                if (board.getTileState(r, c) != TileState::Revealed) {
                    allNonBombsRevealed = false;
                }
            }
        }
    }

    if (allBombsFlagged && allNonBombsRevealed) {
        gameWon = true;
        faceSprite.setTexture(winFaceTexture);

        // Stop the timer on win and change play/pause button
        isPlaying = false;
        playPauseButtonSprite.setTexture(playButtonTexture);

        // Calculate elapsed time
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsedTime = currentTime - startTime - totalPausedDuration;
        int totalSeconds = static_cast<int>(elapsedTime.count());
        int minutes = totalSeconds / 60;
        int seconds = totalSeconds % 60;

        // Add the entry to the leaderboard
        addEntryToLeaderboard(minutes, seconds, playerName);
    }
}
void GameScreen::addEntryToLeaderboard(int minutes, int seconds, const std::string& playerName) {
    // Read existing entries
    std::vector<LeaderboardEntry> entries;
    std::ifstream file("files/leaderboard.txt");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string time, name;
            if (std::getline(ss, time, ',') && std::getline(ss, name)) {
                int min, sec;
                sscanf(time.c_str(), "%d:%d", &min, &sec);
                entries.push_back({min, sec, name});
            }
        }
        file.close();
    }

    // Add the new entry
    entries.push_back({minutes, seconds, playerName});

    // Sort the entries by total time
    std::sort(entries.begin(), entries.end(), [](const LeaderboardEntry &a, const LeaderboardEntry &b) {
        return a.totalSeconds() < b.totalSeconds();
    });

    // Write back the top 5 entries
    std::ofstream outFile("files/leaderboard.txt");
    if (outFile.is_open()) {
        for (int i = 0; i < std::min(5, (int)entries.size()); ++i) {
            outFile << entries[i].minutes << ":" << (entries[i].seconds < 10 ? "0" : "")
                    << entries[i].seconds << "," << entries[i].playerName << std::endl;
        }
        outFile.close();
    }

    // Output the new entry to the console
    std::cout << "New Leaderboard Entry: " << minutes << ":" << (seconds < 10 ? "0" : "") << seconds
              << "," << playerName << std::endl;
}

void GameScreen::resetGame() {
    gameLost = false;
    gameWon = false;
    isPlaying = true;  // Set to true to automatically start the timer
    isPaused = false;
    flagsPlaced = 0;
    totalPausedDuration = std::chrono::duration<float>(0);
    pausedTime = std::chrono::duration<float>(0);  // Correct initialization
    startTime = std::chrono::high_resolution_clock::now();
    faceSprite.setTexture(happyFaceTexture);
    playPauseButtonSprite.setTexture(pauseButtonTexture);
    board = Board(rows, cols, mines);
    openLeaderboard = false;

    // Reinitialize boardSprites with hidden tiles
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

        // Update other game logic here
    }
}

void GameScreen::render(sf::RenderWindow &window) {
    window.clear(sf::Color::White); // Set background to white
    window.draw(gameText);
    drawBoard(window);
    drawButtons(window);
    drawCounter(window);
    drawTimer(window);
}

void GameScreen::drawBoard(sf::RenderWindow &window) {
    for (int r = 0; r < boardSprites.size(); ++r) {
        for (int c = 0; c < boardSprites[r].size(); ++c) {
            if (isPaused) {
                // Display the revealed texture when paused, regardless of the tile state
                boardSprites[r][c].setTexture(revealedTileTexture);
            } else {
                // Reset to hidden texture for tiles that are still hidden
                if (board.getTileState(r, c) == TileState::Hidden) {
                    boardSprites[r][c].setTexture(hiddenTileTexture);
                }
            }

            window.draw(boardSprites[r][c]); // Draw the tile

            // When not paused, handle drawing of mines and numbers
            if (!isPaused) {
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
                        } else {
                            boardSprites[r][c].setTexture(revealedTileTexture);
                        }
                    }
                } else if (board.getTileState(r, c) == TileState::Flagged) {
                    sf::Sprite flagSprite(flagTexture);
                    flagSprite.setPosition(c * 32, r * 32);
                    window.draw(flagSprite);
                }

                // Debug mode: show mines without changing tile texture
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

    std::string counterString = std::to_string(counterValue);
    if (counterString.length() < 3) {
        counterString.insert(0, 3 - counterString.length(), '0');
    }

    if (isNegative) {
        sf::Sprite minusSprite(digitsTexture, sf::IntRect(210, 0, 21, 32));
        minusSprite.setPosition(12, 32 * (rows + 0.5) + 16);
        window.draw(minusSprite);
    }

    for (int i = 0; i < counterString.length(); ++i) {
        int digit = counterString[i] - '0';
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

    std::string minuteString = std::to_string(minutes);
    if (minuteString.length() < 2) {
        minuteString.insert(0, 2 - minuteString.length(), '0');
    }

    std::string secondString = std::to_string(seconds);
    if (secondString.length() < 2) {
        secondString.insert(0, 2 - secondString.length(), '0');
    }

    // Draw minutes
    int startXMinutes = (cols * 32) - 97;
    int startY = 32 * (rows + 0.5) + 16;

    for (int i = 0; i < minuteString.length(); ++i) {
        int digit = minuteString[i] - '0';
        sf::Sprite digitSprite(digitsTexture, sf::IntRect(digit * 21, 0, 21, 32));
        digitSprite.setPosition(startXMinutes + i * 21, startY);
        window.draw(digitSprite);
    }

    // Draw seconds
    int startXSeconds = (cols * 32) - 54;

    for (int i = 0; i < secondString.length(); ++i) {
        int digit = secondString[i] - '0';
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

