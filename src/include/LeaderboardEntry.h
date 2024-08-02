#ifndef LEADERBOARD_ENTRY_H
#define LEADERBOARD_ENTRY_H

#include <string>

struct LeaderboardEntry {
    int minutes;
    int seconds;
    std::string playerName;

    int totalSeconds() const {
        return minutes * 60 + seconds;
    }
};

#endif
