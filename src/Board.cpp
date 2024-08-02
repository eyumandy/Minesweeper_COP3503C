#include "include/Board.h"
#include <cstdlib>
#include <ctime>

Board::Board(int rows, int cols, int mines)
    : rows(rows), cols(cols), mines(mines) {
    initialize();
}

void Board::initialize() {
    board.resize(rows, std::vector<int>(cols, 0));
    tileStates.resize(rows, std::vector<TileState>(cols, TileState::Hidden));

    srand(time(0));
    int placedMines = 0;
    while (placedMines < mines) {
        int r = rand() % rows;
        int c = rand() % cols;
        if (board[r][c] == 0) {
            board[r][c] = 1;
            placedMines++;
        }
    }
}

const std::vector<std::vector<int>>& Board::getBoard() const {
    return board;
}

TileState Board::getTileState(int row, int col) const {
    return tileStates[row][col];
}

void Board::setTileState(int row, int col, TileState state) {
    tileStates[row][col] = state;
}

bool Board::hasMine(int row, int col) const {
    return board[row][col] == 1;
}

int Board::getAdjacentMines(int row, int col) const {
    int mineCount = 0;
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            int r = row + dr;
            int c = col + dc;
            if (r >= 0 && r < rows && c >= 0 && c < cols && board[r][c] == 1) {
                ++mineCount;
            }
        }
    }
    return mineCount;
}