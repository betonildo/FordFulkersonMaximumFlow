#include "Graph.hpp"

Graph::Graph() {
    m_edgesCount = 0;
    m_verticesCount = 0;
}

Graph::Graph(Graph& other) {
    
    m_verticesCount = other.m_verticesCount;
    m_edgesCount = other.m_edgesCount;
    
    for(uint i = 0; i < m_verticesCount; i++) {
    
        if (other.m_graph.find(i) != other.m_graph.end()) {
            for(auto& kvPair : other.m_graph[i]) {
                m_graph[i][kvPair.first] = kvPair.second;
            }            
        }
    }
}

void Graph::set(uint u, uint v, uint w) {
    
    m_edgesCount++;
    
    // use the heigher vertice
    if (m_verticesCount < u) m_verticesCount = u;
    if (m_verticesCount < v) m_verticesCount = v;
    
    m_graph[u][v] = w;
    m_graph[v][u] = w;
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
    
    Graph rGraph((Graph&) *this);
    std::vector<uint> parent;
    
    uint dist = rGraph.dijkstra(s, t, parent);
    
    
    std::cout << "path: " << std::endl;
    for(uint i = t; i != s; i = parent[i]) {
        std::cout << i;
        
        if (i != s) {
            std::cout << " <-- ";
        }
    }
    
    std::cout << std::endl;
    
    return dist;
}

uint Graph::dijkstra(uint s, uint t, std::vector<uint>& parent) {
    
    // mark visited vertices (auto release on scope exit)
    std::unique_ptr<bool[]> visited(new bool[m_verticesCount]);
    std::unique_ptr<uint[]> distances(new uint[m_verticesCount]);
    
    // set all to false
    memset(visited.get(), false, m_verticesCount);
    memset(distances.get(), INF, m_verticesCount);
    
    // clear parents and reserve magic number
    parent.clear();
    parent.reserve(m_verticesCount);
    
    // init heap
    distances[s] = 0;
    HollowHeap<uint, uint> heap(s, 0);
    
    while(!heap.isEmpty()) {
        
        // get next vertice
        uint u = heap.findminKey();
        heap.deleteMin();
        
        // reached to the end
        if (u == t) break;
        
        // get what was not visited yet
        if (!visited[u]) {
            visited[u] = true;
            
            // cicle all neighbors
            for(auto& kvNeighbor : m_graph[u]) {
                
                uint v = kvNeighbor.first;
                uint w = kvNeighbor.second;
                uint c = distances[u] + w;
                
                // if the edge has some weight
                if (w > 0) {
                    // set parent and update distance
                    parent[v] = u;
                    if (distances[v] == INF && c < distances[v]) {
                        distances[v] = c;
                        heap.insert(v, c);
                    }
                    else if (c < distances[v]) {
                        distances[v] = c;
                        heap.update(v, c);
                    }
                }
            }
        }
    }
    
    return distances[t];
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

// get edge cost
uint& Graph::get(uint u, uint v) {
    return m_graph[u][v];
}