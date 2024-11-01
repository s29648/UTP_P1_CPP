// Created by Darya Sharnevich on 25/10/2024.

// Connect Four game rules:
// 6x7 board, 2 players
// The one who first has filled 4 cells in horizontal, vertical or diagonal direction wins

#include "com_example_utpp1_Main.h"
#include <iostream>

struct Point {
    int x;
    int y;

    Point() : x(-1), y(-1) {}
    Point(int r, int c) : x(r), y(c) {}
};

//key -> jobject circle (token); value -> Point struct:
std::unordered_map<jobject, Point, std::hash<jobject>, std::equal_to<>> tokenPositionMap;

class ConnectFourGame {
public:
    // use singleton pattern - there must be only one instance of the class
    // getInstance method returns a reference to the static instance of ConnectFourGame
    // & - returns reference, not a copy
    static ConnectFourGame& getInstance() {
        static ConnectFourGame instance;
        return instance;
    }

    // delete copy constructor and assignment operator
    // prevents copying of the singleton instance
    ConnectFourGame(const ConnectFourGame&) = delete;
    void operator=(const ConnectFourGame&) = delete;

    void resetGame() {
        for (int row = 0; row < NUM_ROWS; ++row) {
            for (int col = 0; col < NUM_COLS; ++col) {
                grid[row][col] = 0;
            }
        }
        currentPlayer = 1;
        gameWon = false;
        winner = 0;
        lastMoveRow = -1;
        lastMoveCol = -1;
    }

    bool isADraw() {
        if(!gameWon && isGridFilled(grid)) return true;
        return false;
    }

    [[nodiscard]] int checkWinner() const {
        return winner;
    }

    [[nodiscard]] int getLastMoveRow() const {
        return lastMoveRow;
    }

    [[nodiscard]] int getLastMoveCol() const {
        return lastMoveCol;
    }

    //returns true if move was valid, otherwise false
    bool dropToken(const int column, jobject token, JNIEnv* env) {
        //boundaries check
        if (column < 0 || column >= NUM_COLS || gameWon) return false;

        //start from the very bottom row and iterate until empty cell is found
        for (int row = NUM_ROWS - 1; row >= 0; --row) {
            if (grid[row][column] == 0) {
                grid[row][column] = currentPlayer;
                lastMoveRow = row;
                lastMoveCol = column;
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

    [[nodiscard]] int getCurrentPlayer() const {
        return currentPlayer;
    }

private:

    int currentPlayer{};
    bool gameWon{};
    int lastMoveRow{};
    int lastMoveCol{};
    int winner {};

    static const int NUM_ROWS = 6;
    static const int NUM_COLS = 7;

    int grid[NUM_ROWS][NUM_COLS]{};

    // constructor is private to prevent external instantiation
    ConnectFourGame() {
        resetGame();
    }

    //count how many tokens are in one of the directions of the line
    [[nodiscard]]
    int countInDirection(const int row, const int column, const int rOffset, const int cOffset, int player)
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
    bool checkLine(const int row, const int column, const int rOffset, const int cOffset) const {
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
    [[nodiscard]] bool hasWinningLine(const int row, const int column) const {
        // offset values:
        //-1: move up for rows, left for columns
        //+1: move down for rows, right for columns
        // 0: no movement in row or column
        return checkLine(row, column, -1, 0)  // (|)
            || checkLine(row, column, 0, -1)  // (-)
            || checkLine(row, column, -1, -1) // (\)
            || checkLine(row, column, -1, 1); // (/)
    }

    //check if all grid is already filled
    static bool isGridFilled(int grid[NUM_ROWS][NUM_COLS]) {
        for (int row = 0; row < NUM_ROWS; ++row) {
            for (int col = 0; col < NUM_COLS; ++col) {
                if (grid[row][col] == 0) {
                    return false;
                }
            }
        }
        return true;
    }

    static void setTokenPosition(JNIEnv* env, jobject token, const int row, const int col) {
        // Create a global reference to the Circle object to prevent garbage collection
        jobject globalTokenRef = env->NewGlobalRef(token);
        tokenPositionMap[globalTokenRef] = Point(row, col);
    }
};

// JNI methods:

JNIEXPORT void JNICALL Java_com_example_utpp1_Main_resetGame(JNIEnv* env, jobject obj) {
    for (auto it = tokenPositionMap.begin(); it != tokenPositionMap.end(); ++it) {
        env->DeleteGlobalRef(it->first);
    }
    tokenPositionMap.clear();
    ConnectFourGame::getInstance().resetGame();
}

JNIEXPORT jboolean JNICALL Java_com_example_utpp1_Main_dropToken(JNIEnv* env, jobject obj, const jint column, jobject token) {
    ConnectFourGame& game = ConnectFourGame::getInstance();
    return static_cast<jboolean>(game.dropToken(static_cast<int>(column), token, env));
}

JNIEXPORT jint JNICALL Java_com_example_utpp1_Main_getCurrentPlayer(JNIEnv* env, jobject obj) {
    return ConnectFourGame::getInstance().getCurrentPlayer();
}

JNIEXPORT jint JNICALL Java_com_example_utpp1_Main_checkWinner(JNIEnv* env, jobject obj) {
    return static_cast<jint>(ConnectFourGame::getInstance().checkWinner());
}

JNIEXPORT jboolean JNICALL Java_com_example_utpp1_Main_isADraw (JNIEnv *, jobject) {
    return static_cast<jboolean>(ConnectFourGame::getInstance().isADraw());
}

JNIEXPORT jint JNICALL Java_com_example_utpp1_Main_getLastMoveRow(JNIEnv* env, jobject obj) {
    return ConnectFourGame::getInstance().getLastMoveRow();
}

JNIEXPORT jobject JNICALL Java_com_example_utpp1_Main_getTokenPosition(JNIEnv* env, jobject obj, jobject token) {
    for (const auto& entry : tokenPositionMap) {
        if (env->IsSameObject(entry.first, token)) {
            Point position = entry.second;

            jclass pointClass = env->FindClass("java/awt/Point");
            jmethodID constructor = env->GetMethodID(pointClass, "<init>", "(II)V");
            jobject javaPoint = env->NewObject(pointClass, constructor, position.x, position.y);

            return javaPoint;
        }
    }

    return nullptr;
}





