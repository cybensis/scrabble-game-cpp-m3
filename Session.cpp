
#include "Session.h"

Session::Session(bool enableColour) {
    this->invalidFile = false;
    this->enableColour = enableColour;
    this->curPlayerIndex = 0;
    if (enableColour) { this->errorMessage = std::string(BOLD_BRIGHT_RED) + "Invalid Input" + std::string(RESET);}
    else { this->errorMessage = "Invalid Input"; }

    // Since you can't declare a 2D vectors size in the header file, it needs to be done here. This loops pushes 15 times,
    // another vector that has 15 char slots in it, all initialised with an empty space.
    for (int i = 0; i < BOARD_SIZE; i++) {
        this->board.push_back(std::vector<char>(BOARD_SIZE, ' '));
    }
    return;
}

Session::Session(std::fstream* loadFile, bool enableColour) {
    this->curPlayerIndex = 0;
    this->numOfPlayers = 0;
    this->enableColour = enableColour;
    if (*loadFile) {
        std::string tempString;
        std::getline(*loadFile, tempString);
        // Could use regex to check for this but I think this looks better and clearer
        if (tempString  == "2" || tempString == "3" || tempString == "4") {
            // Get the players
            this->numOfPlayers = std::stoi(tempString);
            std::string playerName;
            std::string playerScore; 
            std::string playerHand;
            // This loops through all the players in the save game file, only if the file is valid.
            while (this->listOfPlayers.size() < this->numOfPlayers && !this->invalidFile) {
                // The file is set to invalid at the start, then its validated once all these validations have passed
                this->invalidFile = true;
                std::getline(*loadFile, playerName);
                std::getline(*loadFile, playerScore);
                std::getline(*loadFile, playerHand);
                // Player name validation
                if (playerName.length() > 0 && regex_match(playerName, std::regex(CAPS_ONLY_REGEX))) { 
                    // Player score validation
                    if (regex_match(playerScore, std::regex(DIGIT_ONLY_REGEX)) && playerScore.length() > 0) { 
                        // Player hand validation
                        if (isTileListValid(playerHand, MAX_TILES_IN_HAND)) {
                            this->listOfPlayers.push_back(new Player(playerName, playerHand, std::stoi(playerScore)));
                            this->invalidFile = false;
                        }
                    } 
                }
            }
        }
        else { this->invalidFile = true; }
        if (!invalidFile) {
            // Skips the two header lines
            std::getline(*loadFile, tempString);
            std::getline(*loadFile, tempString);
            // Get the board by getting each line, then reading through each char one by one
            for (int i = 0; i < BOARD_SIZE; i++) {
                std::getline(*loadFile, tempString);
                std::stringstream ss(tempString);

                // Vector that each row will be split into
                std::vector<std::string> splitRow;

                // Split the row string based on delimiter and
                // add to vector
                while(std::getline(ss, tempString, '|')){
                    splitRow.push_back(tempString);
                }

                // Discard the first element in vector, which
                // letter of each row.
                splitRow.erase(splitRow.begin());

                /* 
                Vector which will be filled with the middle char
                of every element, which includes the character
                and blank spaces
                */
                std::vector<char> tempCharVector;
                for(unsigned int i = 0; i < splitRow.size(); i++) {
                    tempCharVector.push_back(splitRow.at(i)[1]);
                }
                // Add row to the board vector
                this->board.push_back(tempCharVector);
            }
        }
       
        this->tileBag = new LinkedList();
        std::string tileBagString;
        std::getline(*loadFile, tileBagString);
        if (tileBagString.length() > 0 && isTileListValid(tileBagString, MAX_TILES_IN_BAG)) {
            std::stringstream inputStream(tileBagString); 
            std::vector<std::string> splitTileBagString;

            std::string tileString;

            if (!inputStream.str().empty()) {
                while (std::getline(inputStream, tileString, ',')) { 
                    if (tileString.length() > 0) { splitTileBagString.push_back(tileString); }
                }
            }

            for (int i = 0; i < int(splitTileBagString.size()); i++) {
                int tileScore = std::stoi(splitTileBagString[i].substr(SCORE_INDEX, splitTileBagString[i].size()));
                char tileChar = splitTileBagString[i][CHAR_INDEX];
                Tile tempTile(tileChar, tileScore);
                this->tileBag->addBack(&tempTile);
            }
        } else {
            this->invalidFile = true;
        }

        // Checks the <current player name>
        std::string currentPlayer;
        std::getline(*loadFile, currentPlayer);
        if (currentPlayer.length() == 0 || !regex_match(currentPlayer, std::regex(CAPS_ONLY_REGEX))) {
            this->invalidFile = true;
        }
        else {
            for (int i = 0; i < this->numOfPlayers; i++) {
                if (currentPlayer == this->listOfPlayers.at(i)->getName()) { this->curPlayerIndex = i; }
            }
        }

        if (!invalidFile) {
            for (int i = 0; i < this->numOfPlayers; i++) { this->listOfPlayers.at(i)->setTileBag(this->tileBag); }
            std::cout << std::endl << "Scrabble game successfully loaded" << std::endl;
        }

    } 
    else { this->invalidFile = true; }
    if (this->invalidFile) { std::cout << this->errorMessage << std::endl; }
    loadFile->close();
    return;
}



bool Session::getIfFileInvalid() { return this->invalidFile; }



bool Session::isTileListValid(std::string tiles, int maxSize) {
    bool isValid = true;
    if (tiles.length() > 0) {
        int lengthHand = 0;
        // Modified Split by regex code from https://stackoverflow.com/questions/16749069/c-split-string-by-regex
        std::regex rgx(COMMA_SPLIT_REGEX);
        std::sregex_token_iterator iter(tiles.begin(), tiles.end(), rgx, -1);
        std::sregex_token_iterator end;
        while (iter != end && lengthHand <= maxSize && isValid) {
            std::string tile = *iter;
            ltrim(tile);
            if (lengthHand < maxSize) {
                if (!regex_match(tile, std::regex(TILE_REGEX))) { isValid = false; } // Checks if current tile is <letter>-<number> format
            } 
            else { isValid = false; }
            iter++;
            lengthHand++;
        }
    }
    return isValid;
}



// Code from https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
// trim from start (in place)
void Session::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}



Session::~Session() { delete this->tileBag; }



bool Session::generateTileBag() {
// Need to create the tile bag from ScrabbleTiles.txt first before adding tiles to players hands.
    this->tileBag = new LinkedList();
    bool error = false;
    // Create an ifstream and open the ScrabbleTiles.txt file
    // Check that the file is good, aka that it exists, its readable, etc, then continue
    std::ifstream tilesFile; 
    tilesFile.open("./ScrabbleTiles.txt");
    if (tilesFile.good()) {
        std::string line;
        // Read each line of the file into the line variable.
        while (std::getline(tilesFile, line)) {
            // The tile character should always be located at the first element in the string
            char tileCharacter = line[CHAR_INDEX];
            if ( tileCharacter < 'A' && tileCharacter > 'Z') { error = true; }
            // Yoinked from https://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int, this
            // checks if the substring of line that contains the score is actually an integer.
            if (line.substr(SCORE_INDEX,line.length()).find_first_not_of( "0123456789" ) != std::string::npos) {
                std::cout << "Please make sure there is a valid ScrabbleTiles.txt file in your working directory" << std::endl;
                error = true;
            }
            else {
                int tileScore = std::stoi(line.substr(SCORE_INDEX,line.length()));
                Tile tempTile(tileCharacter, tileScore);
                this->tileBag->addBack(&tempTile);
            }
        }
    }
    else {
        std::cout << "Please make sure there is a valid ScrabbleTiles.txt file in your working directory" << std::endl;
        error = true;        
    }
    return error;
}



void Session::generatePlayers() {

    bool invalidInput = true;
    while (invalidInput) {
        std::string userInput;
        std::cout << "How many people are playing? Range 2-4" << std::endl << "> ";
        std::getline(std::cin, userInput);
        if (userInput == "2" || userInput == "3" || userInput == "4") {
            this->numOfPlayers = std::stoi(userInput);
            invalidInput = false;
        }
        else if (std::cin.eof() || userInput == "^D") { invalidInput = false; this->numOfPlayers = 0; }
        else { std::cout << ERROR_MESSAGE << std::endl << std::endl; }
    }


    int currentUser = 1;
    invalidInput = false;
    while (currentUser <= this->numOfPlayers) {
        if (!invalidInput) { std::cout << "Enter a name for player " << currentUser << " (uppercase characters only)" << std::endl << "> "; } 
        else { std::cout << "> "; }
        invalidInput = false;
        std::string userInput;
        std::getline(std::cin, userInput);
        if (std::cin.eof() || userInput == "^D") { std::cout << std::endl;  this->numOfPlayers = 0; } 
        // Code yoinked from https://stackoverflow.com/questions/48082092/c-check-if-whole-string-is-uppercase
        // This will check if all characters in a string are uppercase characters AND if they are only valid chars (A-Z)
        else if (std::all_of(userInput.begin(), userInput.end(), [](unsigned char c){ return std::isupper(c); }) && userInput.length() > 0) { 
            // Loop through all the created player objects to ensure there are no duplicate names
            for (int i = 0; i < this->listOfPlayers.size(); i++) {
                if (userInput == this->listOfPlayers.at(i)->getName()) { invalidInput = true;}
            }
        } 
        else { invalidInput = true; }

        if (invalidInput) { std::cout << this->errorMessage << std::endl; }
        else { 
            this->listOfPlayers.push_back(new Player(userInput, this->tileBag)); 
            currentUser += 1; 
            std::cout << std::endl;
        }
    }
    return;
}



Player* Session::getCurrentPlayer() { return this->listOfPlayers.at(this->curPlayerIndex); }

void Session::swapCurrentPlayer() {
    this->getCurrentPlayer()->fillHand();
    // This adds +1 to the curPlayerIndex, then the mod ensures if it reaches the size of the list, it goes to 0
    this->curPlayerIndex = (this->curPlayerIndex + 1) % this->numOfPlayers;
    return;
}


Player* Session::getPlayer(int playerIndex) {
    Player* currentPlayer = this->listOfPlayers.at(playerIndex);
    return currentPlayer;
}


BoardVector* Session::getBoard() {
    return &board;
}


// The board positions are already validated as empty before running placeTile(), so no validation needed
void Session::placeTiles(std::vector<int>* handIndexes, std::vector<std::pair<int, int>>* tileCoords) {
    for (unsigned int i = 0; i < handIndexes->size(); i++) {
        Tile* tileInHand = this->getCurrentPlayer()->getTileInHand(handIndexes->at(i));
        // Add tile copy then delete it from the players hand.
        int curRow = tileCoords->at(i).first;
        int curCol = tileCoords->at(i).second;
        board[curRow][curCol] = tileInHand->letter;
    }

    // Since the handIndexes are retrieved based on a FULL HAND, when you delete one, the bigger indexes are no 
    // longer going to be accurate. So to fix this, these two loops will delete the biggest index each loop, which
    // means that no indexes are going to be offset upon deletion.
    int tilesToDelete = handIndexes->size();
    for (int i = 0; i < tilesToDelete; i++) { 
        // The biggestIndex is in regards to the biggest index number in the players hand, but the actual value stored is
        // the element in handIndexes that holds the biggest index.
        int biggestIndex = 0;
        for (unsigned int a = 0; a < handIndexes->size(); a++) {
            biggestIndex = (handIndexes->at(a) > handIndexes->at(biggestIndex)) ? a : biggestIndex;
        }
        // After finding the biggest index in the remaining indexes, delete them from both the hand and handIndexes.
        this->getCurrentPlayer()->getHand()->deleteAt(handIndexes->at(biggestIndex));
        handIndexes->erase(handIndexes->begin() + biggestIndex);
    }
    return;
}



bool Session::positionEmpty(std::pair<int, int> position) {
    bool isEmpty = false;
    // The validTilePlacement() function in gameEngine can pass in positions outside of the boards boundaries,
    // so this checks to make sure it doesn't try and access something like board[-1][15]
    if (position.first >= 0 && position.first < BOARD_SIZE && position.second >= 0 && position.second < BOARD_SIZE) {
        isEmpty = (board[position.first][position.second] == ' ');
    }
    // This check is only needed for the validTilePlacement function when checking the coords around a given coord
    else if (position.first < 0 || position.first >= BOARD_SIZE || position.second < 0 || position.second >= BOARD_SIZE) {
        isEmpty = true;
    }
    return isEmpty;
}


void Session::printPlayersHand(Player* player) {
    LinkedList* playersHand = player->getHand();
    for (int i = 0; i < playersHand->size(); i++ ) {
        Tile* curTile = playersHand->get(i)->tile;
        std::string tile = "";
        // If colour is enabled, then grab the assign the corresponding score colour, otherwise don't assign colours
        // since curTile->letter is a char, the tile string can't be concatenated in one line.
        if (this->enableColour) {
            tile += tilePointColours.at(curTile->value);
            tile += curTile->letter;
            tile += "-" + std::to_string(curTile->value) + RESET;
        }
        else {
            tile += curTile->letter;
            tile += "-" + std::to_string(curTile->value);    
        } 
        // Can't print the "," at the end of the hand printing statement so this if statement checks for the 
        // moment when i == hand size-1 (-1 because of the array index starting at 0) to change the print statement.
        if (i != (playersHand->size() - 1)) { std::cout << tile << ", "; }
        else { std::cout << tile; }
    }
    std::cout << std::endl << std::endl;
    return;
}



int Session::getTileBagSize() {
    return this->tileBag->size();
}



LinkedList* Session::getTileBag() {
    return this->tileBag;
}

int Session::getNumOfPlayers() {
    return this->numOfPlayers;
}