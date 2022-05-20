
#include "LinkedList.h"

LinkedList::LinkedList() :head(nullptr), tail(nullptr), length(0) { // Apparently it is faster doing it this way
//    this->head = nullptr;
//    this->tail = nullptr;
//    this->length = 0;
}

LinkedList::~LinkedList() {
    clear();
}

int LinkedList::size() {
    return this->length;
}

void LinkedList::clear() {
    /*
    ccording to the Linkedlist structure, the pointer
    changes its navigation till it finds a null element
    */
    Node* curNode = this->head;

    while (curNode != nullptr) {
        Node* tmpNode = curNode;
        curNode = curNode->next;
        delete tmpNode;
    }

    this->head = nullptr;
    this->tail = nullptr;
    this->length = 0;
}

Node* LinkedList::get(int i) {
    Node* newNode = this->head;
    while (i < this->length && i > 0) {
        newNode = newNode->next;
        i--;
    }
    return newNode;
}

// Adds a copy tile to the front of the linked list
void LinkedList::addFront(Tile* tile) {
    this->head = new Node(new Tile(*tile), this->head);
    if (this->tail == nullptr) {
        this->tail = this->head;
    }
    this->length++;

}

void LinkedList::addBack(Tile* tile) {
    if (this->length == 0) {
        addFront(tile);
    } else {
        this->tail->next = new Node(new Tile(*tile), nullptr);
        this->tail = this->tail->next;
        this->length++;
    }
}

void LinkedList::deleteFront() {
    if (this->length > 0) {
        Node* tmpNode = this->head;
        this->head = this->head->next; // Move head pointer to next node
        this->tail = (this->head == nullptr) ? nullptr : this->tail; // If there is only one node initially, and it is about to be deleted...
        delete tmpNode;
        this->length--;
    }
}

void LinkedList::deleteBack() {

    if (this->length > 0) {
        if (this->head->next == nullptr) { // If there is only one node in the linked list
            Node* tmpNode = this->head;
            this->head = nullptr;
            this->tail = nullptr;
            delete tmpNode;
        } else {
            // Find the second last node and change its 'next' pointer to null
            Node* prevNode = this->head;
            Node* tmpNode = this->tail;
            while (prevNode->next->next != nullptr) { // Iterate to find the second last node
                prevNode = prevNode->next;
            }
            prevNode->next = nullptr;
            this->tail = prevNode; // Move the tail back
            delete tmpNode; // Delete the initial tail

        }
        this->length--;
    }

}

void LinkedList::addAt(Tile* tile, int i) {
    // Can only add a node before the first index (at index 0), all the way, anywhere, till after the last index
    if (i <= this->length) {
        if (i == 0) { // if insert at index zero, meaning no there is no node before this node
            this->head = new Node(new Tile(*tile), this->head);
        } else { // if insert at index greater than zero, meaning there is/are node(s) before index node
            // 1. Find node before index node. 2. Create a new node and 'next' pointer points to the
            // index node. 3. Node before index node 'next' pointer points to new node
            Node* prevNode = this->head;
            for (int j = 0; j < i - 1; j++) { // Iterate to find the node before the index node
                prevNode = prevNode->next;
            }
            Node* newNode = new Node(new Tile(*tile), prevNode->next); // Create a new node and 'next' pointer points to index node
            prevNode->next = newNode; // 'Node before index node' 'next' pointer points to new node
            this->tail = (newNode->next == nullptr) ? newNode : this->tail;
        }
        this->length++;
    }
}

void LinkedList::deleteAt(int i) {
    // Can only delete a node that is between index 0 and the max index in the linked list
    if (i < this->length && i >= 0) {
        if (i == 0) { // if delete at index zero, meaning no there is no node before this node
            deleteFront();
        } else { // if delete at index greater than zero, meaning there is/are node(s) before index node
            Node* prevNode = this->head;
            for (int j = 0; j < i - 1; j++) { // Iterate to find the node before the index node
                prevNode = prevNode->next;
            }
            Node* dltNode = prevNode->next; // Node to be deleted
            prevNode->next = prevNode->next->next; // From: previous node -> index node -> following node. To: previous node -> following node.
            this->tail = (prevNode->next == nullptr) ? prevNode : this->tail;
            delete dltNode;
            this->length--;
        }
    }
}
