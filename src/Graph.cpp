#include "Graph.hpp"

Graph::Graph() {
    m_edgesCount = 0;
    m_verticesCount = 0;
}

Graph::Graph(Graph& other) {
    
    m_verticesCount = other.m_verticesCount;
    m_edgesCount = other.m_edgesCount;
    
    for(int i = 0; i < m_verticesCount; i++) {
    
        if (other.m_graph.find(i) != other.m_graph.end()) {
            for(auto& kvPair : other.m_graph[i]) {
                m_graph[i][kvPair.first] = kvPair.second;
            }
        }
    }

    m_src = other.m_src;
    m_sink = other.m_sink;
}

void Graph::setSize(int size) {
    m_verticesCount = size;
}

void Graph::set(int u, int v, int w) {
    
    m_edgesCount++;
    
    // use the heigher vertice
    if (m_verticesCount < u) m_verticesCount = u;
    if (m_verticesCount < v) m_verticesCount = v;
    
//    m_graph[v][u] = -w;
    m_graph[u][v] = w;
}

void Graph::unset(int u, int v) {
    m_edgesCount--;
    m_graph[u].erase(v);
}

void Graph::unset(int u) {
    m_verticesCount--;
    m_graph.erase(u);
}

// find the maximum flow
int Graph::ford_fulkerson_max_flow() {
    
    Graph rGraph((Graph&) *this);

    int max_flow = 0;
    int path_flow = 0;

    // while there is a path, the dijkstra resulting is the fattest path
    while((path_flow = rGraph.dijkstraFattestPath()) > 0) {
//        std::cout << "path_flow: " << path_flow << std::endl;
        max_flow += path_flow;
    }

    return max_flow;
}

int Graph::dijkstraFattestPath() {

    std::vector<int> fat;
    std::vector<int> parent;
    fat.resize(m_verticesCount + 1);
    parent.resize(m_verticesCount + 1);

    // set all fat to 0 and parents to 0
    for (int i = 0; i < m_verticesCount + 1; i++) {
        fat[i] = 0;
        parent[i] = 0;
    }

    // source is parent of it self
    int s = m_src;
    int t = m_sink;

    // init heap
    fat[s] = INF;

    typedef std::pair<int, int> WeightVertice;

    class CompareWeightNode {
    public:
        bool operator()(WeightVertice& a, WeightVertice& b) {
            return a.first > b.first;
        }
    };

    typedef std::priority_queue<WeightVertice, std::vector<WeightVertice>, CompareWeightNode> PriorityQueue;

    PriorityQueue queue;
//    NHeap<WeightVertice> queue(2); queue.comparator = [](WeightVertice& a, WeightVertice& b) { return a.first >= b.first; };

    queue.push(std::make_pair(fat[s], s));

    while(!queue.empty()) {

        int u = queue.top().second;
        queue.pop();

        // cicle all neighbors
        for(auto& kvNeighbor : m_graph[u]) {

            int v = kvNeighbor.first;
            int w = kvNeighbor.second;
            int c = (fat[u] < w) ? fat[u] : w;

            // save minimum capacities
            if (fat[v] < c) {
                fat[v] = c;
                parent[v] = u;
                queue.push(std::make_pair(c, v));
            }
        }
    }

    int path_flow = INF;
    for (int v = t; v != s && v != 0; v = parent[v]) {
        path_flow = MIN(path_flow, fat[v]);
    }

    // update residual graph
    for (int v = t; v != s && v != 0 && parent[v] != 0; v = parent[v]) {
        int u = parent[v];
        m_graph[u][v] -= path_flow;
    }

    return path_flow;
}

int Graph::dijkstra(int s, int t, std::vector<int>& parent) {
    
    // mark visited vertices (auto release on scope exit)
    std::unique_ptr<bool[]> visited(new bool[m_verticesCount]);
    std::unique_ptr<int[]> distances(new int[m_verticesCount]);
    
    // set all to false
    for (int i = 0; i < m_verticesCount; i++) {
        visited[i] = false;
        distances[i] = INF;
    }

    // source is parent of it self
    parent[s] = s;

    // init heap
    distances[s] = 0;
    HollowHeap<int, int> heap(0, s);
//    NHeap<Edge> heap(2);
//    heap.insert({s, 0});

//    while(!heap.empty()) {
    while(!heap.isEmpty()) {
        
        // get next vertice
        //int u = heap.getNext().to;
        int u = heap.findminValue();
        heap.deleteMin();

        // reached to the end
//        if (u == t) break;

        // get what was not visited yet
        if (!visited[u]) {
            visited[u] = true;
            
            // cicle all neighbors
            for(auto& kvNeighbor : m_graph[u]) {
                
                int v = kvNeighbor.first;
                int w = kvNeighbor.second;
                int c = distances[u] + w;

                // if the edge has some weight
                if (w > 0) {
                    parent[v] = u;
                    // set parent and update distance
                    if (distances[v] == INF && c < distances[v]) {
                        distances[v] = c;
                        heap.insert(c, v);
//                        heap.insert({v, w});
                    }
                    else if (c < distances[v]) {
                        distances[v] = c;
                        heap.update(c, v);
//                        heap.update({v, w});
                    }
                }
            }
        }
    }
    
    return distances[t];
}

// confirm that exist a path from s to t
bool Graph::breadth_first_transversal(int s, int t) {
    
    // resulting
    bool found = false;
    
    // mark visited vertices (auto release on scope exit)
    std::unique_ptr<bool[]> visited(new bool[m_verticesCount]);

    // set all to false
    memset(visited.get(), false, m_verticesCount);

    // breadth search is made using queues
    std::queue<int> q;

    // start from 'source'
    q.push(s);

    while(!q.empty()) {

        // dequeue
        int v = q.front();
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
int& Graph::get(int u, int v) {
    return m_graph[u][v];
}

void Graph::setSource(int src) {
    m_src = src;
}

void Graph::setSink(int sink) {
    m_sink = sink;
}
