
#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"

class LinkedList {
public:

    LinkedList();
    ~LinkedList();

    int size();
    void clear();
    Node* get(int i);

    void addFront(Tile* tile);
    void addBack(Tile* tile);

    void deleteFront();
    void deleteBack();

    void addAt(Tile* tile, int i);
    void deleteAt(int i);

private:
    Node* head;
    Node* tail;
    int length;
};

#endif // ASSIGN2_LINKEDLIST_H
