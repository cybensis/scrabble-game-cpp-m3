
#ifndef ASSIGN2_GAMEENGINE_H
#define ASSIGN2_GAMEENGINE_H

// When printing out the rows for the board a loop 0-14 is used, 0+65 is the decimal
// value for ASCII A, then 1+65 is B, etc, etc.
#define INT_ASCII_OFFSET       65
// This value represents the maximum amount of chars that can be displayed per column in the column 
// header values, e.g. " 1  " and then " 10 ". This is needed because the column needed have 2 spaces 
// appended, but for double digit numbers this makes it too big, so one space needs to be removed.
#define COL_HEADER_LENGTH       4
#define MAX_MOVES_PER_TURN      7
#define SPACE_DELIMITER        ' '
// #define COORDINATE_DELIMITER   '-'
#define COORDINATE_ROW          0
#define COORDINATE_COL          1
// These values are used when validating user input. If a user enters "place A at C-4", this
// statement is split into 4 components, and the first 4 values represent each of those index locations.
// There is also one for "Done" when entering "place Done"
#define COMMAND_INDEX_LOC       0
#define TILE_INDEX_LOC          1
#define PLACE_AT_INDEX_LOC      2
#define COORDS_INDEX_LOC        3
#define PLACE_CMD_COMPONENTS    4
#define DONE_INDEX_LOC          1
// These two are for the maximum and minimum ranges of the Y coordinate on the board
#define ROW_RANGE_MIN           'A'
#define ROW_RANGE_MAX           'O'
#define USER_PASSED             true
#define USER_DID_NOT_PASS       false
#define BINGO_POINTS            50
#define COORD_LEN_3             3
#define COORD_LEN_2             2
#include "Session.h"
#include <sstream>
#include <fstream>


class GameEngine {
public:

   GameEngine();
   GameEngine(std::fstream* loadFile);
   ~GameEngine();

    // inputController is what controls the entire flow of the game (not including menu)
   void gameController();
   void handlePlayerTurn();
   bool validInput(std::string* input, std::vector<int>* tileHandIndexes, std::vector<std::pair<int, int>>* tileQueueCoords);
   void printBoard();
   // validCoordinates checks the validity of the coordinates provided in the "place _ at _-_"
   // then returns true or false is they are valid or not.
   bool validCoordinates(std::string coordinates);
   bool checkEndConditions();
   // Pass in a copy of coordinates because we want to remove tiles from it 
   bool validTilePlacement(std::vector<std::pair<int,int>>* coordinates);
   // This function is used to check if the board is empty. This is needed since the first tiles 
   // placed onto the board don't need to intersect with an already placed word because there are none. 
   bool boardEmpty(); 


private:
    Session* instanceData;
    int scoreThisTurn;
    Player* currentPlayer;
    bool turnFinished;
    int tilesPlacedThisRound;
    bool quitGame;

   
};

#endif // ASSIGN2_GAMEENGINE_H
