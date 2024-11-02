// Created by Darya Sharnevich on 25/10/2024.

// Connect Four game rules:
// 6x7 board, 2 players
// The one who first has filled 4 cells in horizontal, vertical or diagonal direction wins

#include "ConnectFourGame.h"
#include <iostream>


//key -> jobject circle (token); value -> Point struct:
std::unordered_map<jobject, Point, std::hash<jobject>, std::equal_to<> > tokenPositionMap;

// use singleton pattern - there must be only one instance of the class
// getInstance method returns a reference (&) to the static instance of ConnectFourGame
ConnectFourGame &ConnectFourGame::getInstance() {
    static ConnectFourGame instance;
    return instance;
}

ConnectFourGame::ConnectFourGame() {
    resetGame();
}

void ConnectFourGame::resetGame() {
    for (int row = 0; row < NUM_ROWS; ++row) {
        for (int col = 0; col < NUM_COLS; ++col) {
            grid[row][col] = 0;
        }
    }
    currentPlayer = 1;
    gameWon = false;
    winner = 0;
    lastMoveRow = -1;
}

bool ConnectFourGame::isADraw() {
    if (!gameWon && isGridFilled(grid)) return true;
    return false;
}

[[nodiscard]] int ConnectFourGame::checkWinner() const {
    return winner;
}

[[nodiscard]] int ConnectFourGame::getLastMoveRow() const {
    return lastMoveRow;
}

//returns true if move was valid, otherwise false
bool ConnectFourGame::dropToken(const int column, jobject token, JNIEnv *env) {
    //boundaries check
    if (column < 0 || column >= NUM_COLS || gameWon) return false;

    //start from the very bottom row and iterate until empty cell is found
    for (int row = NUM_ROWS - 1; row >= 0; --row) {
        if (grid[row][column] == 0) {
            grid[row][column] = currentPlayer;
            lastMoveRow = row;
            setTokenPosition(env, token, row, column);

            if (hasWinningLine(row, column)) {
                gameWon = true;
                winner = currentPlayer;
            } else {
                currentPlayer = (currentPlayer == 1) ? 2 : 1;
            }
            return true;
        }
    }
    //when column is full
    return false;
}

[[nodiscard]] int ConnectFourGame::getCurrentPlayer() const {
    return currentPlayer;
}


//count how many tokens are in one of the directions of the line
[[nodiscard]]
int ConnectFourGame::countInDirection(const int row, const int column, const int rOffset, const int cOffset, int player)
const {
    int counter = 0;
    int r = row + rOffset;
    int c = column + cOffset;

    while (
        //boundaries check
        r >= 0 && r < NUM_ROWS
        && c >= 0 && c < NUM_COLS
        //match check
        && grid[r][c] == player) {
        ++counter;
        r += rOffset;
        c += cOffset;

        //no need to iterate further, if there are +3 consecutive token found
        if (counter >= 3) break;
    }
    return counter;
}

// check full given line in both directions
[[nodiscard]]
bool ConnectFourGame::checkLine(const int row, const int column, const int rOffset, const int cOffset) const {
    const int player = grid[row][column];
    if (player == 0) throw std::runtime_error("Not valid player!");

    // we already now that 1 (current) cell is filled by this player
    int counter = 1;
    // add consecutive tokens from positive and negative direction of the line
    counter += countInDirection(row, column, rOffset, cOffset, player);
    counter += countInDirection(row, column, -rOffset, -cOffset, player);

    return counter >= 4;
}

//check all directions for this cell
[[nodiscard]] bool ConnectFourGame::hasWinningLine(const int row, const int column) const {
    // offset values:
    //-1: move up for rows, left for columns
    //+1: move down for rows, right for columns
    // 0: no movement in row or column
    return checkLine(row, column, -1, 0) // (|)
           || checkLine(row, column, 0, -1) // (-)
           || checkLine(row, column, -1, -1) // (\)
           || checkLine(row, column, -1, 1); // (/)
}

//check if all grid is already filled
bool ConnectFourGame::isGridFilled(int grid[NUM_ROWS][NUM_COLS]) {
    for (int row = 0; row < NUM_ROWS; ++row) {
        for (int col = 0; col < NUM_COLS; ++col) {
            if (grid[row][col] == 0) {
                return false;
            }
        }
    }
    return true;
}

void ConnectFourGame::setTokenPosition(JNIEnv *env, jobject token, const int row, const int col) {
    // create a global reference to token to prevent garbage collection
    jobject globalTokenRef = env->NewGlobalRef(token);
    //use this global ref as a key for tokenPositionMap
    tokenPositionMap[globalTokenRef] = Point(row, col);
}
