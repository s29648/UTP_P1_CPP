#ifndef CONNECTFOURGAME_H
#define CONNECTFOURGAME_H

#include <unordered_map>
#include <jni.h>
#include <iostream>

struct Point {
    int x;
    int y;

    Point() : x(-1), y(-1) {}

    Point(const int r, const int c) : x(r), y(c) {}
};

// singleton class for the game logic.
class ConnectFourGame {
public:
    static ConnectFourGame &getInstance();

    // delete copy constructor and assignment operator
    // prevents copying of the singleton instance
    ConnectFourGame(const ConnectFourGame &) = delete;

    void operator=(const ConnectFourGame &) = delete;

    void resetGame();

    bool isADraw();

    [[nodiscard]] int checkWinner() const;

    [[nodiscard]] int getLastMoveRow() const;

    bool dropToken(int column, jobject token, JNIEnv *env);

    [[nodiscard]] int getCurrentPlayer() const;

private:
    // constructor is private to prevent external instantiation
    ConnectFourGame();

    static const int NUM_ROWS = 6;
    static const int NUM_COLS = 7;

    int grid[NUM_ROWS][NUM_COLS]{};
    int currentPlayer{};
    bool gameWon{};
    int lastMoveRow{};
    int winner{};

    [[nodiscard]] int countInDirection(int row, int column, int rOffset, int cOffset,
                                       int player) const;

    [[nodiscard]] bool checkLine(int row, int column, int rOffset, int cOffset) const;

    [[nodiscard]] bool hasWinningLine(int row, int column) const;

    static bool isGridFilled(int grid[NUM_ROWS][NUM_COLS]);

    static void setTokenPosition(JNIEnv *env, jobject token, int row, int col);
};

// global token position map
extern std::unordered_map<jobject, Point, std::hash<jobject>, std::equal_to<> > tokenPositionMap;

#endif
