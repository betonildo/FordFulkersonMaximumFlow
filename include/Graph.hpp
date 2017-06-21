#ifndef GRAPH_H
#define GRAPH_H

#include "definitions.hpp"
#include "utils.hpp"

struct Node;
struct Edge;

struct Node {
    int key;
    int fat;
    std::vector<std::shared_ptr<Edge> > outEdges;
    std::shared_ptr<Node> parent_node;
    std::shared_ptr<Edge> parent_edge;

    Node() {}

    Node(Node* other) {
        key = other->key;
        fat = other->fat;
        outEdges = other->outEdges;
        parent_node = other->parent_node;
        parent_edge = other->parent_edge;
    }

    Node(const Node& other) {
        key = other.key;
        fat = other.fat;
        outEdges = other.outEdges;
        parent_node = other.parent_node;
        parent_edge = other.parent_edge;
    }
};

struct Edge {
    std::shared_ptr<Node> toNode;
    int capacity;

    Edge() {}

    Edge(Edge* other) {
        capacity = other->capacity;
        toNode = other->toNode;
    }

    Edge(const Edge& other) {
        capacity = other.capacity;
        toNode = other.toNode;
    }
};


class Graph {

public:
    Graph();
    Graph(Graph& other);

    void setSize(int size);

    void set(int u, int v, int w);
    void unset(int u, int v);
    void unset(int u);

    // find the maximum flow
    int ford_fulkerson_max_flow();
    
    // find the fattest capacity of the edges
    int dijkstraFattestPath();
    
    // confirm that exist a path from s to t
    bool breadth_first_transversal(int s, int t);
    
    int& get(int u, int v);
    void setSource(int src);
    void setSink(int sink);

    int fatest_path();
    
private:
    std::map<int, std::map<int, int> > m_graph;
    int m_edgesCount;
    int m_verticesCount;

    int m_src;
    int m_sink;
    std::map<int, std::shared_ptr<Node> > nodes;
};

#endif /*GRAPH_H*/
