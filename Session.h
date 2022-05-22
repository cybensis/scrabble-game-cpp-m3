
#ifndef ASSIGN2_SESSION_H
#define ASSIGN2_SESSION_H

#define BOARD_SIZE          15
// When reading in the ScrabbleTiles.txt file, instead of splitting each line on a space, I just 
// use the index value of the string to access it, the character is the first in each line so index 
// 0, then a space at index 1 which we ignore, and score follows immediately after at index 2.
#define CHAR_INDEX           0
#define SCORE_INDEX          2
#define TILE_REGEX           "^[A-Z]-(\\d|10)$"
#define COMMA_SPLIT_REGEX    ","
#define MAX_TILES_IN_HAND    7
#define MAX_TILES_IN_BAG     98
#define CAPS_ONLY_REGEX      "^([A-Z])*$"
#define DIGIT_ONLY_REGEX     "^\\d+$"
#define ERROR_MESSAGE        BOLD_BRIGHT_RED << "Invalid Input" << RESET

#include "Node.h"
#include "LinkedList.h"
#include "Player.h"
#include "Tile.h"
#include "Colours.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>  
#include <cstring>
#include <regex>
#include <algorithm>


typedef std::vector<std::vector<char>> BoardVector;
typedef std::vector<Player*> PlayerList;
//Maybe make a type def for coordinate pairs

class Session {
public:

   Session();
   Session(std::fstream* loadFile);
   ~Session();
   bool generateTileBag();
   void generatePlayers();

   bool getIfFileInvalid();
   int getTileBagSize();
   Player* getCurrentPlayer();
   Player* getPlayer(int playerIndex);
   BoardVector* getBoard();
   LinkedList* getTileBag();
   int getNumOfPlayers();

   void swapCurrentPlayer();
   void placeTiles(std::vector<int>* handIndexes, std::vector<std::pair<int, int>>* coordinates);
   // Could change this to getPlayersHand so its only GameEngine controlling the printing
   void printPlayersHand(Player* player);
   bool positionEmpty(std::pair<int, int> position);


private:
   PlayerList listOfPlayers;
   int numOfPlayers;
   LinkedList* tileBag;
   // This initialises the 2D board vector as a 15x15 array, and all spaces are initialised with a single whitespace
   BoardVector board;
   // Holds an index value in reference to listOfPlayers, for the current player
   int curPlayerIndex;
   bool invalidFile;
   bool isTileListValid(std::string tiles, int maxSize);
   void ltrim(std::string &s);

   
};

#endif // ASSIGN2__Session_H
