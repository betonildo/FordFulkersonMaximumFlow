#include "Graph.hpp"

Graph::Graph() {
    m_edgesCount = 0;
    m_verticesCount = 0;
}

void Graph::set(uint u, uint v, uint w) {
    
    m_edgesCount++;
    
    // use the heigher vertice
    if (m_verticesCount < u) m_verticesCount = u;
    if (m_verticesCount < v) m_verticesCount = v;
    
    m_graph[u][v] = w;
}

void Graph::unset(uint u, uint v) {
    m_edgesCount--;
    m_graph[u].erase(v);
}

void Graph::unset(uint u) {
    m_verticesCount--;
    m_graph.erase(u);
}

// find the maximum flow
uint Graph::ford_fulkerson_max_flow(uint s, uint t) {
    
}

// confirm that exist a path from s to t
bool Graph::breadth_first_transversal(uint s, uint t) {
    
    // resulting
    bool found = false;
    
    // mark visited vertices (auto release on scope exit)
    std::unique_ptr<bool[]> visited(new bool[m_verticesCount]);

    // set all to false
    memset(visited.get(), false, m_verticesCount);

    // breadth search is made using queues
    std::queue<uint> q;

    // start from 'source'
    q.push(s);

    while(!q.empty()) {

        // dequeue
        uint v = q.front();
        q.pop();
        
        // return true if v (the next vertice) is sink t
        if (v == t) {
            found = true;
            break;
        }

        // get vertice and weight
        for(auto& ew : m_graph[v]) {

            // if not visited, push it to the queue
            if (!visited[ew.first]) {
                visited[ew.first] = true;
                q.push(ew.first);
            }
        }
    }

    // the path was not found
    return found;
}

