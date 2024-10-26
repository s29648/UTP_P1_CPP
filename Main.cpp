// Created by Darya Sharnevich on 25/10/2024.

// Connect Four game rules:
// 6x7 board, 2 players
// The one who first has filled 4 cells in horizontal, vertical or diagonal direction wins

// #include "connect_four.h"

#include "Main.h"
#include <iostream>
#include <vector>


class Main {

public:
    static const int ROWS = 6;
    static const int COLS = 7;
    std::vector<std::vector<int>> grid;
    int currentPlayer;

    //constructor
    Main() {
        grid.resize(ROWS, std::vector<int>(COLS, 0)); //6x7 matrix with all 0 cells
        currentPlayer = 1;  // possible values for currentPlayer: 1,2
    }

    // one move logic
    bool dropToken(int column) {
        if (column < 0 || column >= COLS) return false;  //check if the column exist

        for (int row = ROWS - 1; row >= 0; row--) {
            if (grid[row][column] == 0) {  // if the cell is empty
                grid[row][column] = currentPlayer; // fill the cell
                bool hasWinner = whoIsWinner(row, column); // Check for winner
                currentPlayer = (currentPlayer == 1) ? 2 : 1; // Toggle player
                return hasWinner; // Return if the game is won
            }
        }
        return false;  // all cells in the column already filled
    }

    // check game winner
    bool whoIsWinner(int row, int column) {
        // player identifier
        int player = grid[row][column];
        if (player == 0) return false;

        return hasFilledDirection(row, column);
    }


private:
    //directions to be checked (dRow/dCol values): 1 (positive), -1 (negative), 0
    bool checkDirection(int row, int column, int dRow, int dCol) {
        // player identifier
        int player = grid[row][column];
        int count = 1; // Start with the current cell

        // Check forward in the direction
        for (int i = 1; i < 4; i++) {
            int newRow = row + dRow * i;
            int newCol = column + dCol * i;
            if (
                //boundaries check
                newRow < 0
                || newRow >= ROWS
                || newCol < 0
                || newCol >= COLS
                //match check
                || grid[newRow][newCol] != player)
                break;
            count++;
        }

        // Check backward in the opposite direction
        for (int i = 1; i < 4; i++) {
            int newRow = row - dRow * i;
            int newCol = column - dCol * i;
            if (
                //boundaries check
                newRow < 0
                || newRow >= ROWS
                || newCol < 0
                || newCol >= COLS
                //match check
                || grid[newRow][newCol] != player)
                break;
            count++;
        }

        if (count >= 4) {
            std::cout << "Player " << player << " wins!" << std::endl;
            return true;
        }
        return false;
    }

    bool hasFilledDirection(int row, int column) {
        return checkDirection(row, column, 1, 0)   //Dir: "-"
            ||  checkDirection(row, column, 0, 1)  //Dir: "|"
            ||  checkDirection(row, column, 1, 1)  //Dir: "/"
            ||  checkDirection(row, column, 1, -1); //Dir: "\"
    }
};



JNIEXPORT jlong JNICALL Java_Main_createGame(JNIEnv *env, jobject obj) {
return reinterpret_cast<jlong>(new Main());
}

JNIEXPORT jintArray JNICALL Java_Main_getBoard(JNIEnv *env, jobject obj, jlong Pointer) {
    Main* game = reinterpret_cast<Main*>(Pointer);

    // Create a Java int array to hold the board data
    jintArray result = env->NewIntArray(Main::ROWS * Main::COLS);
    if (!result) return nullptr;

    jint* boardArray = env->GetIntArrayElements(result, 0);

    // Flatten the 2D board grid into a 1D array
    for (int row = 0; row < Main::ROWS; row++) {
        for (int col = 0; col < Main::COLS; col++) {
            boardArray[row * Main::COLS + col] = game->grid[row][col];
        }
    }

    // Release the int array elements and commit changes to result array
    env->ReleaseIntArrayElements(result, boardArray, 0);

    return result;
}

JNIEXPORT jboolean JNICALL Java_Main_dropToken(JNIEnv *env, jobject obj, jlong Pointer, jint column) {
    Main* game = reinterpret_cast<Main*>(Pointer);
    return static_cast<jboolean>(game->dropToken(column));
}







