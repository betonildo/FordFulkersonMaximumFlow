#include "HollowHeap.h"

// jump for corruption search
jmp_buf jump;

void segv (int sig) {
    longjmp (jump, 1); 
}

int memcheck (void *x) 
{
  volatile char c = 0;
  int illegal = c;

  signal (SIGSEGV, segv);

  if (!setjmp (jump))
    c = *(char *) (x);
  else
    illegal = 1;

  signal (SIGSEGV, SIG_DFL);

  return (illegal);
}

Item* make_element(U32 vertice) {
    Item* item = (Item*)calloc(1, sizeof(Item));
    item->vertice = vertice;
    item->node = NULL;
    return item;
}

Node* make_heap(Item* e, U32 key) {
    Node* n = (Node*) calloc(1, sizeof(Node));
    n->item = e;
    n->key = key;
    n->firstChild = NULL;
    n->nextSibling = n;
    n->rank = 0;
    n->N = 1; // metadata for delete-min con
    e->node = n; // e->node has to point to this node
    return n;
}

Item* findmin(Node* h) {
    return h != NULL ? h->item : NULL;
}

Node* getmin(Node* h) {
    return h;
}

Node* insert(Item* e, U32 key, Node* h) {
    return insert(e, key, h, NULL);
}

Node* insert(Item* e, U32 key, Node* h, U32* swaps) {
    Node* rh = meld(make_heap(e, key), h, swaps);
    rh->N += 1;
    return rh;
}

Node* decrease_key(Item* e, U32 key, Node* h) {
    return decrease_key(e, key, h, NULL);
}

Node* decrease_key(Item* e, U32 key, Node* h, U32* swaps) {
    Node* u = e->node;
    Node* v = make_heap(e, key);
    v->rank = MAX(0, u ? u->rank - 2 : 0);

    if (u && memcheck(u) && u->rank >= 2 && u->rank < INF) {
        v->firstChild = u->firstChild->nextSibling->nextSibling;
        u->firstChild->nextSibling->nextSibling = NULL;
    }

    Node* rh = meld(v, h, swaps);
    rh->N += 1;
    return rh;
}

Node* delete_element(Item* e, Node* h) {
    if (e->node->item != NULL) {
        free(e->node->item);
        e->node->item = NULL;
    }

    if (e->node == h)
        return delete_min(h);
    return h;
}

Node* delete_min(Node* h) {
    return delete_min(h, NULL);
}

Node* delete_min(Node* h, U32* swaps) {
    if (h == NULL) return NULL;
    //if (h->item != NULL) free(h->item);
    h->item = NULL;
    
    // M is defined following colorary
    // to reduce the number N, used to discover M, each hollowNode that is
    // removed from heap, decrements one
    double phi = PHI;
    double lres = logBase(h->N, phi);
    U32 M = ceil(lres);
    
    Node** Roots = NULL;
    // Only reserve R if it will be used
    if (M > 0) Roots = (Node**) calloc(M, sizeof(Node*));
    // init all R's
    for (U32 i = 0; i < M; i++) Roots[i] = NULL;
    
    // count how many nodes was distroyed
    U32 numberOfHollowNodesRemoved = 0;

    Node* root = h;
    
    do {
        Node* nextRoot = root->nextSibling;
        root = link_heap(root, Roots, &numberOfHollowNodesRemoved, swaps);
        root = nextRoot;
    }
    while(root != h && root != NULL);

    h = NULL;
    for (U32 i = 0; i < M; i++) {
        if (Roots[i] != NULL) {
            Roots[i]->nextSibling = Roots[i];
            h = meld(h, Roots[i], swaps);
        }
    }
    // free R of Nodes 
    if (Roots != NULL) free(Roots);
    if (h) {
        // update number of nodes eliminated/removed
        h->N = MAX(0, h->N - numberOfHollowNodesRemoved);
    }
    return h;
}

Node* meld(Node* h1, Node* h2) {
    return meld(h1, h2, NULL);
}

Node* meld(Node* h1, Node* h2, U32* swaps) {
    if (h1 == NULL) return h2;
    if (h2 == NULL) return h1;

    // SWAP to create a cycle list, probably of roots
    Node* tmp = h2->nextSibling;
    h2->nextSibling = h1->nextSibling;
    h1->nextSibling = tmp;

    // meld does the swaps too
    incrementSwaps(swaps);

    if (h1->key <= h2->key) return h1;
    else return h2;
}

Node* link_heap(Node* h, Node** R, U32* removeCount) {
    return link_heap(h, R, removeCount, NULL);
}

//TODO: MUST RETURN NEW heap
Node* link_heap(Node* h, Node** Roots, U32* removeCount, U32* swaps) {
    // h is hollow
    if (h->item == NULL) {
        
        Node* r = h->firstChild;
        while (r != NULL) {
            Node* rn = r->nextSibling;
            link_heap(r, Roots, removeCount, swaps);
            r = rn;
        }
        
        // if (h) free(h);
        h = NULL;
        return h;
    }
    // Roots that are not reserved yet or some node that was deleted DONT enter in this place
    else if (Roots != NULL && h != NULL && memcheck(h) && h->rank < INF) {
        U32 i = h->rank;
        Node* cursorToRoot = Roots[i];
        while (cursorToRoot != NULL && memcheck(cursorToRoot)) {
            // link with
            h = link(h, cursorToRoot);
            Roots[i] = NULL;
            i = i + 1;
            // increment swaps on each link, 
            // because each link change winner and loser
            incrementSwaps(swaps);
        }
        Roots[i] = h;
    }

    return h;
}

Node* link(Node* t1, Node* t2) {

    if (t1->key <= t2->key)
        return makechild(t1, t2);
    else
        return makechild(t2, t1);
}

Node* makechild(Node* w, Node* l) {
    l->nextSibling = w->firstChild;
    w->firstChild = l;
    w->rank = w->rank + 1;
    return w;
}

void incrementSwaps(U32* swaps) {
    if (swaps) (*swaps) += 1;
}

Node* delete_heap(Node* h) {
    if (h == NULL) return NULL;
    
    Node* next = h;
    while(next != NULL) {
        Node* toDelete = next;
        next = next->nextSibling;
        if (toDelete != NULL) {
            delete_heap(toDelete->firstChild);
            free(toDelete);
        }
    }

    return NULL;
}