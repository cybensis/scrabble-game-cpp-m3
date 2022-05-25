#include "GameEngine.h"

GameEngine::GameEngine(bool enableColour)
{
    this->instanceData = new Session(enableColour);
    bool tileBagError = this->instanceData->generateTileBag();
    if (!tileBagError) {
        this->instanceData->generatePlayers();
        // If the player exits during creating players, the number of players should be 0
        if (this->instanceData->getNumOfPlayers() > 0) {
            this->currentPlayer = this->instanceData->getCurrentPlayer();
            this->scoreThisTurn = 0;
            this->turnFinished = false;
            this->tilesPlacedThisRound = 0;
            this->quitGame = false;
        }
    }
}

GameEngine::GameEngine(std::fstream* loadFile, bool enableColour) {
    this->quitGame = false;
    this->instanceData = new Session(loadFile, enableColour);
    if (!this->instanceData->getIfFileInvalid()) {
        this->currentPlayer = this->instanceData->getCurrentPlayer();
        this->scoreThisTurn = 0;
        this->turnFinished = false;
        this->tilesPlacedThisRound = 0;
    }
    else {
        std::cout << "Please check the format of your save file" << std::endl;
    }
}


GameEngine::~GameEngine()
{
    delete this->instanceData;
}


void GameEngine::gameController() {
    // This checks if the tilebag threw an error, or the players were never made due to an error or ^D
    if (!this->instanceData->getIfFileInvalid() && this->instanceData->getNumOfPlayers() > 0) {
        std::cout << "Let's Play!" << std::endl << std::endl;
        while (!quitGame) {
            std::cout << this->currentPlayer->getName() << ", it's your turn" << std::endl;
            for (int i = 0; i < this->instanceData->getNumOfPlayers(); i++) {
                std::cout << "Score for " << this->instanceData->getPlayer(i)->getName() << ": " << this->instanceData->getPlayer(i)->getScore() << std::endl;
            }
            printBoard();
            std::cout << "Your hand is" << std::endl;
            this->instanceData->printPlayersHand(this->currentPlayer);
            
            // handlePlayerTurn handles the 7 moves a player can make per turn, then everything is reset for the next player
            handlePlayerTurn();
            bool gameFinished = checkEndConditions();
            if (gameFinished) {
                std::cout << "Game over" << std::endl;
                int highestScore = 0;
                int winnerIndex = DRAW;
                // This for loop is a two in one. First if prints the name and score of each player, and at the same time it finds who has the highest score
                for (int i = 0; i < this->instanceData->getNumOfPlayers(); i++) {
                    std::cout << "Score for " << this->instanceData->getPlayer(i)->getName() << ": " << this->instanceData->getPlayer(i)->getScore() << std::endl;
                    if (this->instanceData->getPlayer(i)->getScore() > highestScore) { 
                        highestScore =  this->instanceData->getPlayer(i)->getScore();
                        winnerIndex = i;
                    }
                    else if (this->instanceData->getPlayer(i)->getScore() == highestScore) {
                        winnerIndex = DRAW;
                    }
                 }
                if (this->instanceData->enableColour) { std::cout << BOLD_BRIGHT_GREEN; }
                // Quick if else to check if its a draw or not
                if (winnerIndex == DRAW) { std::cout << "This game ended in a draw!" << std::endl; }
                else { std::cout << this->instanceData->getPlayer(winnerIndex)->getName() << " is the winner!" << std::endl; }
                std::cout << RESET;
                this->quitGame = true;
            }
            // Reset all the variables that need to be reset
            this->currentPlayer->addScore(this->scoreThisTurn);
            this->instanceData->swapCurrentPlayer();
            this->currentPlayer = this->instanceData->getCurrentPlayer();
            // If the tile bag is empty and a player has already passed their turn twice, they shouldn't be allowed to play anymore.
            // This is only necessary if there are more than 2 players.
            while (this->instanceData->getTileBagSize() == 0 && this->currentPlayer->getConsecutivePasses() >= 2) {
                this->instanceData->swapCurrentPlayer();
                this->currentPlayer = this->instanceData->getCurrentPlayer();
            }
            this->turnFinished = false;
            this->tilesPlacedThisRound = 0;
            this->scoreThisTurn = 0;
            std::cout << std::endl;

        }
    }
    return;

}

bool GameEngine::checkEndConditions() {
    bool conditionsMet = false;
    // This checks for the end game conditions, first if the tile bag is empty, then if checks all users to see how many have passed twice or are out of tiles
    int playersOut = 0;
    if (this->instanceData->getTileBagSize() == 0) {
        for (int i = 0; i < this->instanceData->getNumOfPlayers(); i++) {
            if (this->instanceData->getPlayer(i)->getHand()->size() == 0 || this->instanceData->getPlayer(i)->getConsecutivePasses() >= 2) {
                playersOut += 1;
            }

        }
    }
    // If the number of users who are out of the game, are equal to the total number of players minus 1, then that means there is only one player
    // who is still able to play, therefore the game should end.
    if (playersOut == (this->instanceData->getNumOfPlayers() - 1)) { conditionsMet = true; }
    return conditionsMet;
}



void GameEngine::handlePlayerTurn() {
    std::vector<std::pair<int, int>> queueCoords;  
    std::vector<int> queueHandIndexes;  
    while ( !this->turnFinished ) {
        bool errorCondition = false;
        // Special operation: Bingo conditions
        if (this->tilesPlacedThisRound == MAX_MOVES_PER_TURN) {
            if (validTilePlacement(&queueCoords) || boardEmpty()) { 
                std::cout << std::endl << "BINGO!!!" << std::endl;
                this->scoreThisTurn += BINGO_POINTS;
                this->instanceData->placeTiles(&queueHandIndexes, &queueCoords);
                this->turnFinished = true;
            }
            else { errorCondition = true; }
        } else {
            std::cout << "> ";
            std::string userInput;
            // getline is needed because just using std::cin >> userInput will ignore everything after whitespaces
            std::getline(std::cin, userInput);
            if (std::cin.eof() || userInput == "^D") {
                this->turnFinished = true;
                this->quitGame = true;
            }
            else if (!validInput(&userInput, &queueHandIndexes, &queueCoords)) { std::cout << this->instanceData->errorMessage << std::endl; }
            else if (this->turnFinished && this->tilesPlacedThisRound > 0 && !boardEmpty()) {
                // validTilePlacement uses queueCoords to confirm that the user is intersecting with an already existing word
                if (validTilePlacement(&queueCoords)) { this->instanceData->placeTiles(&queueHandIndexes, &queueCoords); }
                else { errorCondition = true; }
            }
            // This is for the very first turn of a new game
            else if (this->turnFinished && this->tilesPlacedThisRound > 0 && boardEmpty()) {
                this->instanceData->placeTiles(&queueHandIndexes, &queueCoords);
            }
        }
        // This runs if the tiles coordinates are not valid to place
        if (errorCondition) {
            std::cout << this->instanceData->errorMessage << std::endl;
            queueCoords.clear();
            queueHandIndexes.clear();
            this->tilesPlacedThisRound = 0;
            this->scoreThisTurn = 0;
            this->turnFinished = false;
        }
    }
    return;
}



bool GameEngine::validTilePlacement(std::vector<std::pair<int,int>>* coordinates) {
    bool tilePlacementValid = false;
    // This statement loops through all the tiles and if it can find even one tile that connects to an already existing tile on the board
    // then it is valid
    for (unsigned int i = 0; i < coordinates->size(); i++) {
        int row = coordinates->at(i).first;
        int col = coordinates->at(i).second;
        // The position can be -1 or 15 (out of bounds) but positionEmpty() checks for this
        std::pair<int, int> north(row - 1, col);
        std::pair<int, int> east(row, col + 1);
        std::pair<int, int> south(row + 1, col);
        std::pair<int, int> west(row, col - 1);
        // The loop checks every coordinate given, and this if statement checks all those coordinates to see if any of them intersect with an 
        // already existing tile thats been placed. This is needed because a new word must be connected to one already placed down.
        // NOTE: This doesn't cover pretty much any of the edge cases, like someone can place one tile not connected, then one tile connected
        // and it will count. I tried including validation for all the edge cases but its pretty difficult to cover everything
        if (!this->instanceData->positionEmpty(north) || !this->instanceData->positionEmpty(east) || !this->instanceData->positionEmpty(south) || !this->instanceData->positionEmpty(west)) {
            tilePlacementValid = true;
        }
    }

    return tilePlacementValid;
}



bool GameEngine::validInput(std::string* input, std::vector<int>* queueHandIndexes, std::vector<std::pair<int, int>>* queueCoords) {
    std::stringstream inputStream(*input); 
    std::vector<std::string> splitInput; 
    std::string tempString;
    // Start by assuming there are errors, then set to true if input is fully validated. Helps remove repetitive else statements
    bool noErrors = false;
    // Make sure the input isn't empty 
    if (!inputStream.str().empty()) {
        // Idea taken from https://java2blog.com/split-string-space-cpp/
        // This while loop will take the input thats been read into a string stream, then it will read through the stream
        // until it finds a space, then all the characters up until that space are placed into tempString, which is then
        // pushed onto the splitInput vector.
        while (std::getline(inputStream, tempString, SPACE_DELIMITER)) { 
            // We need to allow for any amount of spaces to be entered, so this checks that once the delimiter picks up a 
            // space, that there are actually chars before it, and it wasn't just a string of spaces which makes it empty.
            if (tempString.length() > 0) { splitInput.push_back(tempString); }
        }
        std::string givenCommand = splitInput.at(COMMAND_INDEX_LOC);
        if (splitInput.size() > 4 || splitInput.size() == 0) { noErrors = false; }
        else if (givenCommand == "place") {
            // The tilesPlacedThisRound != MAX_MOVES_PER_TURN isn't necessary because you can't place a tile if your hand is empty
            // but it's more efficient since it doesn't have to run through all this code to figure the hand is empty. 
            if (splitInput.size() == PLACE_CMD_COMPONENTS && this->tilesPlacedThisRound != MAX_MOVES_PER_TURN) {
                int tileHandIndex = TILE_NOT_FOUND;
                char chosenTileChar;
                if (splitInput.at(TILE_INDEX_LOC).length() == 1) {
                    // The splitInput.at(..) returns a string, but we only want the char given, and since a string is an array
                    // of chars, using [0] we can get the char value of the provided tile, given the input is only 1 character long
                    chosenTileChar = splitInput.at(TILE_INDEX_LOC)[0];
                    tileHandIndex = this->instanceData->getCurrentPlayer()->findTile(chosenTileChar, queueHandIndexes);
                }
                std::string coordinates = splitInput[COORDS_INDEX_LOC];
                // Here we check first if the given tile is actually in the players hand, then we check if the third component
                // is equal to "at" like it always should be, then we check the coordinates provided are valid.
                if (tileHandIndex != TILE_NOT_FOUND && splitInput[PLACE_AT_INDEX_LOC] == "at" && validCoordinates(coordinates)) {
                    // Using [0] again because we only want the char, and if it passes validCoordinates, then we know its only 1 char,
                    // then once we have the char, we can subtract INT_ASCII_OFFSET from it, which will return an int in range 0-14
                    // int rowCoord = coordinates.substr(0, coordinates.find(COORDINATE_DELIMITER))[0] - INT_ASCII_OFFSET;
                    int rowCoord = coordinates[COORDINATE_ROW] - INT_ASCII_OFFSET;
                    // Need coordinates.find(COORDINATE_DELIMITER) + 1 because we want it to be AFTER The '-'
                    // int colCoord = std::stoi(coordinates.substr(coordinates.find(COORDINATE_DELIMITER) + 1, coordinates.size()));
                    int colCoord = std::stoi(coordinates.substr(COORDINATE_COL, coordinates.size()));
                    std::pair<int,int> coordinatePair(rowCoord, colCoord);
                    // This will only work if the location on the board isn't already taken, and the player hasn't already entered this coordinate
                    if ( this->instanceData->positionEmpty(coordinatePair) && std::find(queueCoords->begin(), queueCoords->end(), coordinatePair) == queueCoords->end()) {
                        queueHandIndexes->push_back(tileHandIndex);
                        queueCoords->push_back(coordinatePair);
                        this->scoreThisTurn += this->instanceData->getCurrentPlayer()->getTileInHand(tileHandIndex)->value;
                        this->tilesPlacedThisRound += 1;
                        noErrors = true;
                    }
                }
            }
            // If all 4 components aren't there, they could still be trying to enter "place Done", but you can only enter "place Done"
            // if you've placed something down first, otherwise you'll have to use "pass".
            else if (splitInput[DONE_INDEX_LOC] == "Done" && this->tilesPlacedThisRound != 0) { 
                this->instanceData->getCurrentPlayer()->setConsecutivePasses(USER_DID_NOT_PASS);
                this->turnFinished = true;
                noErrors = true;    
            }

        }
        else if (givenCommand == "replace" && splitInput.size() == 2) {
            // Make sure that the tile they've given is only 1 character long, and that they haven't placed a tile this round.
            if (splitInput.at(TILE_INDEX_LOC).length() == 1 && this->tilesPlacedThisRound == 0) { 
                // No need to validate input beyond ".length() == 1" because findTile() will return -1 regardless if its invalid
                int tileHandIndex = this->instanceData->getCurrentPlayer()->findTile(splitInput.at(TILE_INDEX_LOC)[0]);
                if (tileHandIndex != -1) {
                    this->instanceData->getCurrentPlayer()->replaceTile(tileHandIndex);
                    this->turnFinished = true;
                    noErrors = true;
                }
            }
        }
        else if (givenCommand == "pass") {
            // Check that only one command was given, AND that they haven't placed any tiles yet. If they have placed a tile, then
            // they need to use "place Done" to end their turn.
            if (splitInput.size() == 1 && this->tilesPlacedThisRound == 0) {
                this->instanceData->getCurrentPlayer()->setConsecutivePasses(USER_PASSED);
                this->turnFinished = true;
                noErrors = true;
            }
        }
        else if (givenCommand == "save") { 
            if(splitInput.size() == 2) {
                createSaveFile(splitInput.at(1));
                std::cout << "Successfully saved!" << std::endl;
                noErrors = true;
            }
        } 
        // Added the ability to cancel the "place" command in case a wrong coordinate or wrong tile is chosen.
        else if (givenCommand == "cancel") {
            if (splitInput.size() == 1 && this->tilesPlacedThisRound > 0) {
                noErrors = true;
                this->tilesPlacedThisRound = 0;
                queueHandIndexes->clear();
                queueCoords->clear();
                this->scoreThisTurn = 0;
            }
        }
        else if (givenCommand == "help") {
            if (splitInput.size() == 1) { printHelp(); noErrors = true; }
            else if (splitInput.size() == 2) { printHelp(splitInput.at(1)); noErrors = true; }
        }
        else if (givenCommand == "quit") { this->quitGame = true; this->turnFinished = true; noErrors = true;}
    }
    return noErrors;

}




bool GameEngine::validCoordinates(std::string coordinates) {
    bool noErrors = true;

    // Since the row coordinate can only ever be one character long, we only bother with getting the first character.
    // If they try and supply something like AA15, the column coordinate validation will pick up on the additional A 
    char rowCoord = coordinates[COORDINATE_ROW];
    std::string colCoord = coordinates.substr(COORDINATE_COL, coordinates.size());

    // row coordinate validation to check if its in range of A-O
    if (rowCoord < ROW_RANGE_MIN ||  rowCoord > ROW_RANGE_MAX) { noErrors = false; }
    // Make sure the column coord isn't more than 2 chars, or empty, then check if the first char is a valid 
    if ( colCoord.size() > 2 || colCoord.size() == 0 || !std::isdigit(colCoord[0]) ) { noErrors = false; }
    // The else if first checks if the digit is two characters long, then it checks if the first character is a 0 (because 
    // they can try place C at A-01), then it checks if the second char is an int or not.
    else if (colCoord.size() == 2 && (colCoord[0] == '0' || !std::isdigit(colCoord[1]) ) ) {  noErrors = false; }
    // The two statements above validate the colCoord as a valid integer, so stoi is safe to use
    else if (std::stoi(colCoord) < 0 || std::stoi(colCoord) >= BOARD_SIZE) { noErrors = false; }
    return noErrors;
}




void GameEngine::printBoard() {
    BoardVector* board = this->instanceData->getBoard();
    // Start by printing out the Column coordinates, starting with 4 spaces for the row coordinates,
    // then start printing out " 1  ", " 2  ", etc. Note that the second space after each number is
    // needed because of the "|" in between each coordinate, but if the number is double digits,
    // it has the 2nd space stripped off
    std::cout << "   ";
    for (int i = 0; i < BOARD_SIZE; i++) {
        std::string colHeader = " " + std::to_string(i) + "  ";
        std::cout << colHeader.substr(0, COL_HEADER_LENGTH);
    }
    // Now printing out the ---- under the column numbers, this time we only need 3 spaces because the
    // dashes start when the "|" start, whereas column numbers start after "|"
    std::cout << std::endl << "  ";
    for (int i = 0; i < BOARD_SIZE; i++) {
        // Exactly 4 dashes are needed per column
        std::cout << "----";
    }
    std::cout << "-"; // Add one more dash to the end
    std::cout << std::endl;
    // After the board column coordinates have been set, start printing out the actual board contents
    for (int i = 0; i < BOARD_SIZE; i++) {
        std::cout << char(i + INT_ASCII_OFFSET) << " |";
        for (int a = 0; a < BOARD_SIZE; a++) {
            if (this->instanceData->enableColour) { std::cout << " " << BOLD_BRIGHT_CYAN << board->at(i).at(a) << RESET << " |"; }
            else { std::cout << " " << board->at(i).at(a) << " |"; }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    return;
}


bool GameEngine::boardEmpty() {
    bool isBoardEmpty = true;
    // Efficient way of checking if the board is empty is checking if any players have score
    for (int i = 0; i < this->instanceData->getNumOfPlayers(); i++) {
        if (this->instanceData->getPlayer(i)->getScore() > 0) { isBoardEmpty = false; }
    }
    return isBoardEmpty;
}



void GameEngine::createSaveFile(std::string fileName) {
//The name of the file is the first element of the vector
                std::ofstream save;
                save.open(fileName);
                LinkedList* hand;
                save << this->instanceData->getNumOfPlayers() << "\n";
                // Read the 2 players data and add it to the save file
                for (int a = 0; a < this->instanceData->getNumOfPlayers(); a++) {
                    hand = instanceData->getPlayer(a)->getHand();
                    save << instanceData->getPlayer(a)->getName() << '\n';
                    save << instanceData->getPlayer(a)->getScore() << '\n';
                    //in order to get the elements of the hand (tiles), we do an iteration through the whole vector
                    for (int i = 0; i < hand->size(); i++ ) {
                        Tile* curTile = hand->get(i)->tile;
                        if (i != (MAX_TILES_IN_HAND - 1)) { save << curTile->letter << "-" << curTile->value << ","; }
                        else { save << curTile->letter << "-" << curTile->value << '\n'; }
                    }
                }
            //We need to construct the board in the file
            //After declaration, we write the column names which are less than the board size (length)
            BoardVector* board = instanceData->getBoard();

            save << "   ";
            for (int i = 0; i < BOARD_SIZE; i++) {
                std::string colHeader = " " + std::to_string(i) + "  ";
                save << colHeader.substr(0, COL_HEADER_LENGTH);
            }

            save << std::endl << "  ";
            for (int i = 0; i < BOARD_SIZE; i++) {
                save << "----";
            }
            save << "-" << std::endl;

            //In order to save position on the board (e.g. C1), we do a nested for loop which respectively gives the ascii alphabet and the horizontal position
            for (int i = 0; i < BOARD_SIZE; i++) {
                save << char(i + INT_ASCII_OFFSET) << " |";
                for (int a = 0; a < BOARD_SIZE; a++) {
                    save << " " << board->at(i).at(a) << " |";
                }
            save << std::endl;
            }

            LinkedList* tileBag = instanceData->getTileBag();
            //Tilebag is processed/saved like player hand
            for (int i = 0; i < tileBag->size(); i++ ) {
                Tile* curTile = tileBag->get(i)->tile;
                if (i != (tileBag->size() - 1)) {
                    save << curTile->letter << "-" << curTile->value << ",";
                }
                else {
                    save << curTile->letter << "-" << curTile->value << '\n';
                }
            }
            //At the end, for the last line, we save the current player names
            save << currentPlayer->getName();
}

void GameEngine::printHelp() {
    std::cout << "Available commands: place, replace, pass, save, quit" << std::endl;
    std::cout << "Type \"help commandname\" for more info" << std::endl;
    std::cout << std::endl;
    return;
}

void GameEngine::printHelp(std::string command) {
    if (this->instanceData->enableColour) { std::cout << BOLD_BRIGHT_GREEN; }
    if (command == "place") {
        std::cout << "\"place\" Command" << std::endl;
        std::cout << "      Usage: Used to place tiles on the board" << std::endl;
        std::cout << "      Example: place Y at C10" << std::endl;
        std::cout << "\"place Done\" Command" << std::endl;
        std::cout << "      Usage: Used to finish your turn and place all selected tiles down" << std::endl;
        std::cout << "      Example: place Done" << std::endl;
        std::cout << "\"cancel\" Command" << std::endl;
        std::cout << "      Usage: Used to undo all the \"place\" commands entered" << std::endl;
        std::cout << "      Example: cancel" << std::endl;
    }
    else if (command == "replace") {
        std::cout << "\"replace\" Command" << std::endl;
        std::cout << "      Usage: Used to replace a tile in the players hand" << std::endl;
        std::cout << "      Example: replace E" << std::endl;
    }
    else if (command == "pass") {
        std::cout << "\"pass\" Command" << std::endl;
        std::cout << "      Usage: Used to pass the turn without placing any tiles down" << std::endl;
        std::cout << "      Example: pass" << std::endl;
    }
    else if (command == "save") {
        std::cout << "\"save\" Command" << std::endl;
        std::cout << "      Usage: Used to save the state of the current game" << std::endl;
        std::cout << "      Example: save filename" << std::endl;
    }
    else if (command == "quit") {
        std::cout << "\"quit\" Command" << std::endl;
        std::cout << "      Usage: Used to quit the game" << std::endl;
        std::cout << "      Example: quit" << std::endl;
    }
    else {
        if (this->instanceData->enableColour) { std::cout << BOLD_BRIGHT_RED; }
        std::cout << command << " is not a valid command!" << RESET << std::endl;
        std::cout << "Available commands: place, replace, pass, save, quit" << std::endl;
    }
    std::cout << RESET << std::endl;
    return;



}