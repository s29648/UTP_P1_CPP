## UTP project #1

Connect four (6x7 grid) game application C++ library

JNI native methods available:

- boolean dropToken(int column, Circle token) - if the column is valid and not filled yet, sets a token position to the 
first available row in the column, sets new token reference with its coordinates to the hash map, then check if 
this move led to win or not.
- int getCurrentPlayer() - returns current player
- int checkWinner() - returns winner number if it is, 0 otherwise
- boolean isADraw() - checks if grid is already filled but game is not won
- int getLastMoveRow() - returns last move row
- Point getTokenPosition(Circle token) - returns column and row coordinates of the token
- void resetGame() - resets game, clears global references to tokens for previous game, clears the hash map
