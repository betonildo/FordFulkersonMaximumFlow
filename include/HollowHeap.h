#ifndef HOLLOWHEAP_H
#define HOLLOWHEAP_H

#include <stdlib.h>
#include <math.h>
#include "definitions.hpp"
#include "utils.hpp"

typedef struct Item Item;
typedef struct Node Node;

struct Node {
    Item* item;
    U32 key;
    Node* firstChild;
    Node* nextSibling;
    U32 rank;
    U32 N;
};

struct Item {
    Node* node;
    U32 vertice;
};

Item* make_element(U32 vertice);
Node* make_heap(Item* e, U32 key); //O(1)
Item* findmin(Node* h); // O(1)
Node* getmin(Node* h); //O(1)
Node* insert(Item* e, U32 key, Node* h); //O(1)
Node* insert(Item* e, U32 key, Node* h, U32* swaps); //O(1)
Node* decrease_key(Item* e, U32 key, Node* h); //O(1)
Node* decrease_key(Item* e, U32 key, Node* h, U32* swaps); //O(1)
Node* delete_element(Item* e, Node* h); // log(N)
Node* delete_min(Node* h); // log(N)
Node* delete_min(Node* h, U32* swaps); // log(N)
Node* meld(Node* h1, Node* h2); //O(1)
Node* meld(Node* h1, Node* h2, U32* swaps); //O(1)
Node* link_heap(Node* h, Node** R, U32* removeCount); // log(N)
Node* link_heap(Node* h, Node** R, U32* removeCount, U32* swaps); // log(N)
Node* link(Node* t1, Node* t2); //O(1)
Node* makechild(Node* w, Node* l); //O(1)
void incrementSwaps(U32* swaps);
Node* delete_heap(Node* h);
void segv (int sig);
int memcheck (void *x);


#endif /*HOLLOWHEAP_H*/
