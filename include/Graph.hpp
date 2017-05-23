#ifndef GRAPH_H
#define GRAPH_H

#include "definitions.hpp"
#include "HollowHeap.hpp"
#include "nheap.hpp"
#include "edge.hpp"

class Graph {

public:
    Graph();
    Graph(Graph& other);

    void set(int u, int v, int w);
    void unset(int u, int v);
    void unset(int u);

    // find the maximum flow
    int ford_fulkerson_max_flow(int s, int t);
    
    int dijkstraMaxPathFlow(int s, int t, std::vector<int>& parent);

    // 
    int dijkstra(int s, int t, std::vector<int>& parent);    
    
    // confirm that exist a path from s to t
    bool breadth_first_transversal(int s, int t);
    
    int& get(int u, int v);
    
private:
    std::map<int, std::map<int, int>> m_graph;
    int m_edgesCount;
    int m_verticesCount;

};

#endif /*GRAPH_H*/
