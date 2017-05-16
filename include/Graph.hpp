#ifndef GRAPH_H
#define GRAPH_H

#include "definitions.hpp"

class Graph {

public:
    Graph();

    void set(uint u, uint v, uint w);
    void unset(uint u, uint v);
    void unset(uint u);

    // find the maximum flow
    uint ford_fulkerson_max_flow(uint s, uint t);
    
    // confirm that exist a path from s to t
    bool breadth_first_transversal(uint s, uint t);
    
private:
    std::map<uint, std::map<uint, uint>> m_graph;
    uint m_edgesCount;
    uint m_verticesCount;

};

#endif /*GRAPH_H*/
