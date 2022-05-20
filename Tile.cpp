
#include "Tile.h"

Tile::Tile(Letter letter, Value value){
    this->letter = letter;
    this->value = value;
}

Tile::Tile(Tile& other){
    this->letter = other.letter;
    this->value = other.value;
}

Tile::~Tile(){
    // Left blank.
}