#include "include/GameScreen.h"
#include <iostream>

GameScreen::GameScreen(unsigned int windowWidth, unsigned int windowHeight, int rows, int cols, int mines)
    : windowWidth(windowWidth), windowHeight(windowHeight), rows(rows), cols(cols), mines(mines), board(rows, cols, mines), isPlaying(true), gameLost(false), debugMode(false), gameWon(false), isPaused(false), flagsPlaced(0), totalPausedDuration(0), pausedTime(0) {
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

    startTime = std::chrono::high_resolution_clock::now();
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
            // Toggle play/pause button
            if (isPlaying) {
                playPauseButtonSprite.setTexture(playButtonTexture);
                isPaused = true;
                pauseStart = std::chrono::high_resolution_clock::now();
            } else {
                playPauseButtonSprite.setTexture(pauseButtonTexture);
                isPaused = false;
                totalPausedDuration += std::chrono::high_resolution_clock::now() - pauseStart;
                startTime == std::chrono::high_resolution_clock::now() - pausedTime;
            }
            isPlaying = !isPlaying;
        }
        if (faceSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            std::cout << "Clicked on Face button" << std::endl;
            resetGame();
        }
        if (debugButtonSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            std::cout << "Clicked on Debug button" << std::endl;
            debugMode = !debugMode;
        }
        if (leaderboardButtonSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            std::cout << "Clicked on Leaderboard button" << std::endl;
            // Add logic for Leaderboard button
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
    for (int r = 0; r < board.getBoard().size(); ++r) {
        for (int c = 0; c < board.getBoard()[r].size(); ++c) {
            if (board.hasMine(r, c) && board.getTileState(r, c) != TileState::Flagged) {
                return;
            }
        }
    }
    gameWon = true;
    faceSprite.setTexture(winFaceTexture);
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

    // Reinitialize boardSprites with hidden tiles
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            boardSprites[r][c].setTexture(hiddenTileTexture);
            boardSprites[r][c].setPosition(c * 32, r * 32);
        }
    }
}

void GameScreen::update() {
    // Update game logic here
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
            window.draw(boardSprites[r][c]);
            if (board.getTileState(r, c) == TileState::Revealed || (debugMode && board.hasMine(r, c))) {
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

    int elapsedSeconds = static_cast<int>(elapsedTime.count());

    std::string timerString = std::to_string(elapsedSeconds);
    if (timerString.length() < 3) {
        timerString.insert(0, 3 - timerString.length(), '0');
    }

    for (int i = 0; i < timerString.length(); ++i) {
        int digit = timerString[i] - '0';
        sf::Sprite digitSprite(digitsTexture, sf::IntRect(digit * 21, 0, 21, 32));
        digitSprite.setPosition((windowWidth - 97) + i * 21, 32 * (rows + 0.5) + 16);
        window.draw(digitSprite);
    }
}