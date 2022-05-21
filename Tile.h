
#ifndef ASSIGN2_TILE_H
#define ASSIGN2_TILE_H

// Define a Letter type
typedef char Letter;

// Define a Value type
typedef int Value;


class Tile {
public:
   Letter letter;
   Value  value;
   Tile(Letter letter, Value value);
   Tile(Tile& other);
   ~Tile();
};

#endif // ASSIGN2_TILE_H