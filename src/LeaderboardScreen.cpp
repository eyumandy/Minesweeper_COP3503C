#include "include/LeaderboardScreen.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm> // For std::sort

LeaderboardScreen::LeaderboardScreen(unsigned int windowWidth, unsigned int windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight),
      window(sf::VideoMode(windowWidth, windowHeight), "Leaderboard") {
    if (!font.loadFromFile("files/font.ttf")) {
        std::cerr << "Error loading font" << std::endl;
    }

    leaderboardText.setFont(font);
    leaderboardText.setString("Leaderboard");
    leaderboardText.setCharacterSize(24);
    leaderboardText.setFillColor(sf::Color::White);
    leaderboardText.setStyle(sf::Text::Bold);

    sf::FloatRect textRect = leaderboardText.getLocalBounds();
    leaderboardText.setOrigin(textRect.left + textRect.width / 2.0f,
                              textRect.top + textRect.height / 2.0f);
    leaderboardText.setPosition(windowWidth / 2.0f, 50);

    // Load leaderboard entries from file
    loadLeaderboard();
}

void LeaderboardScreen::run() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            handleEvent(window, event);
        }
        update();
        render(window);
    }
}

void LeaderboardScreen::handleEvent(sf::RenderWindow &window, sf::Event &event) {
    if (event.type == sf::Event::Closed) {
        window.close();
    }
}

void LeaderboardScreen::update() {
    // Sort the leaderboard entries by score
    std::sort(entries.begin(), entries.end(), [](const LeaderboardEntry &a, const LeaderboardEntry &b) {
        return a.totalSeconds() < b.totalSeconds();
    });

    // Keep only the top 5 entries
    if (entries.size() > 5) {
        entries.resize(5);
    }
}

void LeaderboardScreen::render(sf::RenderWindow &window) {
    window.clear(sf::Color::Blue); // Set background to blue
    window.draw(leaderboardText);

    // Display leaderboard entries
    for (size_t i = 0; i < entries.size(); ++i) {
        sf::Text entryText;
        entryText.setFont(font);
        entryText.setCharacterSize(20);
        entryText.setFillColor(sf::Color::White);

        std::ostringstream oss;
        oss << (i + 1) << ". " << entries[i].minutes << ":"
            << (entries[i].seconds < 10 ? "0" : "") << entries[i].seconds
            << "," << entries[i].playerName;

        entryText.setString(oss.str());
        entryText.setPosition(50, 100 + i * 30); // Adjust position as needed
        window.draw(entryText);
    }

    window.display();
}

void LeaderboardScreen::addEntry(const LeaderboardEntry &entry) {
    entries.push_back(entry);
    update(); // Sort and keep only top 5
    saveLeaderboard();
}

void LeaderboardScreen::loadLeaderboard() {
    std::ifstream file("src/files/leaderboard.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open leaderboard file." << std::endl;
        return;
    }

    entries.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string time, name;
        if (std::getline(ss, time, ',') && std::getline(ss, name)) {
            int minutes, seconds;
            sscanf(time.c_str(), "%d:%d", &minutes, &seconds);
            entries.push_back({minutes, seconds, name});
        }
    }
    file.close();
}

void LeaderboardScreen::saveLeaderboard() {
    std::ofstream file("src/files/leaderboard.txt");
    if (!file.is_open()) {
        std::cerr << "Could not open leaderboard file for writing." << std::endl;
        return;
    }

    int maxEntries = std::min(5, (int)entries.size());
    for (int i = 0; i < maxEntries; ++i) {
        file << entries[i].minutes << ":" << (entries[i].seconds < 10 ? "0" : "")
             << entries[i].seconds << "," << entries[i].playerName << std::endl;
    }
    file.close();
}