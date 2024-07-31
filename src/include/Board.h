#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <SFML/Graphics.hpp>

enum class TileState { Hidden, Revealed, Flagged };

class Board {
public:
    Board(int rows, int cols, int mines);
    void initialize();
    const std::vector<std::vector<int>>& getBoard() const;
    TileState getTileState(int row, int col) const;
    void setTileState(int row, int col, TileState state);
    int getAdjacentMines(int row, int col) const;
    bool hasMine(int row, int col) const;

private:
    int rows;
    int cols;
    int mines;
    std::vector<std::vector<int>> board;
    std::vector<std::vector<TileState>> tileStates;
};

#endif