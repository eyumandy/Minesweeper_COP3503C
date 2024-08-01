#ifndef LEADERBOARD_ENTRY_H
#define LEADERBOARD_ENTRY_H

#include <string>

struct LeaderboardEntry {
    int minutes;
    int seconds;
    std::string playerName;

    // Calculate total time in seconds for easier comparison
    int totalSeconds() const {
        return minutes * 60 + seconds;
    }
};

#endif
