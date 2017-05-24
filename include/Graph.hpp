#ifndef GRAPH_H
#define GRAPH_H

#include "definitions.hpp"
#include "HollowHeap.hpp"

struct Node;
struct Edge;

struct Node {
    int key;
    int fat;
    std::vector<std::shared_ptr<Edge> > outEdges;
    std::shared_ptr<Node> parent_node;
    std::shared_ptr<Edge> parent_edge;
};

struct Edge {
    std::shared_ptr<Node> toNode;
    int capacity;
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
    
    int dijkstraFattestPath(std::vector<int>& parent);
    int fattest_path();
    // 
    int dijkstra(int s, int t, std::vector<int>& parent);    
    
    // confirm that exist a path from s to t
    bool breadth_first_transversal(int s, int t);
    
    int& get(int u, int v);
    void setSource(int src);
    void setSink(int sink);
    
private:
    std::map<int, std::vector<std::pair<int, int> > > m_graph;
    int m_edgesCount;
    int m_verticesCount;

    int m_src;
    int m_sink;
    std::map<int, std::shared_ptr<Node> > nodes;
};

#endif /*GRAPH_H*/
