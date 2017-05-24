#ifndef HOLLOWHEAP_HPP
#define HOLLOWHEAP_HPP

#include "utils.hpp"
#include "definitions.hpp"


template< typename T >
struct array_deleter
{
  void operator ()( T const * p)
  {
    delete[] p;
  }
};

template<class Key, class Value>
class HollowHeap {

public:

    std::function<bool(Key a, Key b)> comparison = [&](Key a, Key b) -> bool {
        return a <= b;
    };

    class Node;
    class Element;

    typedef std::shared_ptr<Node> NodeSPtr;
    typedef std::shared_ptr<Element> ElementSPtr;
    typedef std::shared_ptr<Node[]> NodeSPtrArray;

    class Element {

    public:
        Value value;
        NodeSPtr node;

        Element(Value value) {
            this->value = value;
        }

    };

    class Node {

    public:
        Key key;
        ElementSPtr element;
        NodeSPtr firstChild;
        NodeSPtr nextSibling;
        uint rank;

        Node(std::shared_ptr<Element>& element, Key key) {
            this->element = element;
            this->key = key;
            firstChild = nullptr;
            nextSibling = nullptr;
            rank = 0;
        }

        Node() {

        }
    };

    HollowHeap(Key k, Value v) {
        ElementSPtr ePtr(new Element(v));
        NodeSPtr nPtr(new Node(ePtr, k));
        ePtr->node = m_headNode = nPtr;
        m_nodesCount = 0;
        m_nodesCount += 1;
    }

    inline void insert(Key k, Value v) {
        ElementSPtr elementPtr(new Element(v));
        NodeSPtr nodePtr(new Node(elementPtr, k));
        elementPtr->node = nodePtr;
        nodePtr->element = elementPtr;
        m_headNode = meld(m_headNode, nodePtr);
        m_nodesCount += 1;
    }
    
    inline void update(Key k, Value v) {
        ElementSPtr ePtr(new Element(v));
        decreaseKey(ePtr, k);
    }

    inline void decreaseKey(ElementSPtr e, Key k) {
        NodeSPtr u = e->node;
        NodeSPtr v(new Node(e, k));
        v->rank = MAX(0, u ? u->rank - 2 : 0);

        if (u && u->rank >= 2) {
            v->firstChild = u->firstChild->nextSibling->nextSibling;
            u->firstChild->nextSibling->nextSibling = nullptr; // maybe the last line did that
        }

        m_headNode = meld(v, m_headNode);
        m_nodesCount += 1;
    }

    inline void deleteElement(ElementSPtr& e) {
        if (e->node->element) e->node->element = nullptr;
        if (e->node == m_headNode) deleteMin();
    }

    inline void deleteMin() {

        if (m_headNode == nullptr) return;
        if (m_headNode->element != nullptr) m_headNode->element = nullptr;

        // M is defined following colorary
        // to reduce the number N, used to discover M, each hollowNode that is
        // removed from heap, decrements one
        double lres = logBase(m_nodesCount, PHI);
        uint M = ceil(lres) + 2;
        Make2Pot(&M);

        std::vector<NodeSPtr> Roots;
        Roots.resize(M);
        for (uint i = 0; i < M; i++) Roots[i] = nullptr;

        uint hollowNodesCount = 0;

        NodeSPtr root = m_headNode;
        NodeSPtr nextRoot;
        do {
            nextRoot = root->nextSibling;
            root = linkHeap(root, Roots, hollowNodesCount);
            root = nextRoot;

        }while(root != m_headNode && root && root->nextSibling != nextRoot);

        m_headNode = nullptr;

        for (uint i = 0; i < M; i++) {
            NodeSPtr cursor = Roots[i];
            if (cursor) {
                cursor->nextSibling = cursor;
                m_headNode = meld(m_headNode, cursor);
            }
        }

        m_nodesCount = MAX(0, m_nodesCount - 1);
    }

    inline bool isEmpty() {
        return !m_headNode;
    }

    inline Value findminValue() {
        return findmin()->value;
    }
    
    inline Key findminKey() {
        return getmin()->key;
    }

    inline ElementSPtr getNextElement() {
        return findmin();
    }

    inline ElementSPtr findmin() {
        if (m_headNode) return m_headNode->element;
        else return nullptr;
    }

    inline NodeSPtr getmin() {
        return m_headNode;
    }

    inline friend std::ostream& operator<<(std::ostream& os, const ElementSPtr& element) {
        if (element) os << element->value;
        else os << "null";
        return os;
    }

private:
    NodeSPtr m_headNode;
    uint m_nodesCount; // count for help define M on delete-min

    NodeSPtr meld(NodeSPtr h1, NodeSPtr h2) {
        if (!h1) return h2;
        if (!h2) return h1;

        // swap for first elements
        if (!h1->nextSibling && !h2->nextSibling) {
            h1->nextSibling = h2;
            h2->nextSibling = h1;
        }
        else if (h1->nextSibling && !h2->nextSibling) {
            h2->nextSibling = h1->nextSibling;
            h1->nextSibling = h2;
        }
        else if (!h1->nextSibling && h2->nextSibling) {
            h1->nextSibling = h2->nextSibling;
            h2->nextSibling = h1;
        }
        else {
            // Swap for cycle list creation NORMAL
            NodeSPtr tmp = h2->nextSibling;
            h2->nextSibling = h2->nextSibling;
            h1->nextSibling = tmp;
        }

        if (comparison(h1->key, h2->key)) return h1;
        else return h2;
    }

    NodeSPtr linkHeap(NodeSPtr h, std::vector<NodeSPtr>& Roots, U32& hollowNodesCount) {

        if (
                h->firstChild &&
                h->firstChild->firstChild &&
                h->firstChild == h->firstChild->firstChild)
            return h;

        if (h->element == nullptr) {
            NodeSPtr root = h->firstChild;
            while(root) {
                NodeSPtr nextRoot = root->nextSibling;
                linkHeap(root, Roots, hollowNodesCount);
                root = nextRoot;
            }

            h = nullptr;
        }
        else {
            uint i = h->rank;
            while(i < Roots.size() && Roots[i] != nullptr) {
                h = link(h, Roots[i]);
                Roots[i] = nullptr;
                i = i + 1;
                hollowNodesCount++;
            }

            if (i < Roots.size())
                Roots[i] = h;
        }

        return h;
    }

    NodeSPtr link(NodeSPtr h1, NodeSPtr h2) {
        if (!h1) return h2;
        if (!h2) return h1;
        if (h2.use_count() == 0) return h1;
        if (h1.use_count() == 0) return h2;
        if (comparison(h1->key, h2->key)) return makeChild(h1, h2);
        else return makeChild(h2, h1);
    }

    NodeSPtr makeChild(NodeSPtr w, NodeSPtr l) {
        l->nextSibling = w->firstChild;
        w->firstChild = l;
        w->rank = w->rank + 1;
        return w;
    }
    
};

#endif /*HOLLOWHEAP_HPP*/
