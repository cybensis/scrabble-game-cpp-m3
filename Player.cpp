
#include "Player.h"

Player::Player(std::string playerName, LinkedList* tileBag) {
    this->playerHand = new LinkedList();
    this->playerScore = 0;
    this->consecutivePasses = 0;
    this->playerName = playerName;
    this->tileBag = tileBag;
    // This generates a randomizer using the current time as the seed, which is adequate for this implementation. 
    // NOTE: Keep this commented out for testing purposes so the randomizer is predictable
    // seededRandomizer = std::mt19937(time(nullptr));
    this->seededRandomizer = std::mt19937(1);
    // This loop assigns 7 (MAX_TILES_IN_HAND) tiles randomly to the players hand
    for (int i = 0; i < MAX_TILES_IN_HAND; i++) {
        // seededRandomizer() will generate a different random number every time seededRandomizer() is called, but the number 
        // will be something big like 884430747, so to use it to randomly access LinkedList indexes, we take that number and 
        // get the remainder of BIG_NUMBER % TILE_BAG_SIZE, which will always return an element in bounds of the tilebag size.
        int randomIndex = (this->seededRandomizer() % this->tileBag->size());
        // If the tilebag isnt empty, add the tile from the tile bag to the players hand, then remove it from the tile bag
        if (this->tileBag->size() > 0) {
            this->playerHand->addFront(this->tileBag->get(randomIndex)->tile);
            tileBag->deleteAt(randomIndex);
        }
    }
    return;
}

Player::Player(std::string playerName, std::string playerHand, int playerScore) {
    this->playerName = playerName;
    this->playerScore = playerScore;
    this->playerHand = new LinkedList();
    
    // Generate player hand from string
    std::stringstream inputStream(playerHand); 
    std::vector<std::string> splitInput; 
    std::string tempString;

    if (!inputStream.str().empty()) {
        while (std::getline(inputStream, tempString, ',')) { 
            if (tempString.length() > 0) { splitInput.push_back(tempString); }
        }
    }

    for (int i = 0; i < int(splitInput.size()); i++) {
        int tileScore = std::stoi(splitInput[i].substr(2, splitInput[i].size()));
        char tileChar = splitInput[i][0];
        Tile tempTile(tileChar, tileScore);
        this->playerHand->addBack(&tempTile);
    }
}

Player::~Player() {
    delete this->playerHand;
}

void Player::setTileBag(LinkedList* tileBagPtr) {
    this->tileBag = tileBagPtr;
}

void Player::addScore(int pointsToAdd) {
    this->playerScore = this->playerScore + pointsToAdd;
    return;
}



void Player::fillHand() {
    // The tiles to add is based off the max amount of tiles in a hand, minus the current tiles in hand
    int tilesToAdd = MAX_TILES_IN_HAND - this->playerHand->size();
    int tilesAdded = 0;
    // While there are still tiles in the tilebag, and there are more tiles to add, keep adding them.
    while (this->tileBag->size() > 0 && tilesAdded < tilesToAdd) {
        int randomIndex = (this->seededRandomizer() % this->tileBag->size());
        this->playerHand->addBack(this->tileBag->get(randomIndex)->tile);
        tileBag->deleteAt(randomIndex);
        tilesAdded += 1;
    }
    return;
} 

// void Player::addOneTile() {
//     // The tiles to add is should be one extra than what they have
//     int tilesToAdd = 1;
//     int tilesAdded = 0;
//     // While there are still tiles in the tilebag, and there are more tiles to add, keep adding them.
//     while (this->tileBag->size() > 0 && tilesAdded < tilesToAdd) {
//         int randomIndex = (this->seededRandomizer() % this->tileBag->size());
//         this->playerHand->addBack(this->tileBag->get(randomIndex)->tile);
//         tileBag->deleteAt(randomIndex);
//         tilesAdded += 1;
//     }
// }

void Player::replaceTile(int tileIndex) {
    this->playerHand->deleteAt(tileIndex);
    // Get a random index with seedRandomizer, then use it to pull and delete a random tile from the tile bag
    int randomIndex = (this->seededRandomizer() % this->tileBag->size());
    this->playerHand->addBack(this->tileBag->get(randomIndex)->tile);
    this->tileBag->deleteAt(randomIndex);
    return;
}



int Player::findTile(char tileChar) {
    // Search through the playerHand LinkedList until the tileInHand has been found by checking for the letter, 
    // then return the index location
    int tileIndex = TILE_NOT_FOUND;
    int i = 0;
    bool tileFound = false;
    while (i < this->playerHand->size() && !tileFound) {
        if (this->playerHand->get(i)->tile->letter == tileChar) {
            tileIndex = i;
            tileFound = true; // To end loop after finding the tile
        }
        i++;
    }
    return tileIndex;
}

int Player::findTile(char tileChar, std::vector<int>* usedIndexes) {
    int tileIndex = TILE_NOT_FOUND;
    int i = 0;
    bool tileFound = false;
    while (i < this->playerHand->size() && !tileFound) {
        // this checks that the tile this loop matches the tile in hand the player wants AND it also checks that they aren't trying to
        // use a tile in their hand thats already been used before
        if (this->playerHand->get(i)->tile->letter == tileChar && std::find(usedIndexes->begin(), usedIndexes->end(), i) == usedIndexes->end()) {
            tileIndex = i;
            tileFound = true; // To end loop after finding the tile
        }
        i++;
    }
    return tileIndex;
}



std::string Player::getName() {
    return this->playerName;
}


int Player::getScore() {
    return this->playerScore;
}

LinkedList* Player::getHand() {
    return this->playerHand;
}

Tile* Player::getTileInHand(int handIndex) {
    return this->playerHand->get(handIndex)->tile;
}

int Player::getConsecutivePasses() {
    return this->consecutivePasses;
}

void Player::setConsecutivePasses(bool userPassedTurn ) {
    if (userPassedTurn) { this->consecutivePasses += 1; }
    else { this->consecutivePasses = 0; }
}
