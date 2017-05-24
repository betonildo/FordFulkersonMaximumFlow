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
}

void Graph::setSize(int size) {
    m_verticesCount = size;
}

void Graph::set(int u, int v, int w) {
    
    m_edgesCount++;
    
    // use the heigher vertice
    if (m_verticesCount < u) m_verticesCount = u;
    if (m_verticesCount < v) m_verticesCount = v;
    
    m_graph[u][v] = w;
    m_graph[v][u] = 0;
//    m_graph[0][u] = w;
//    m_graph[0][v] = w;



    // try find u and v nodes that already has being added to vertices
    std::shared_ptr<Edge> e(new Edge);

    std::shared_ptr<Node> nu = nodes[u];
    std::shared_ptr<Node> nv = nodes[v];

    if (nu == nullptr)
        nu.reset(new Node);
    if (nv == nullptr)
        nv.reset(new Node);

    e->capacity = w;
    e->toNode = nv;
    nv->key = v;
    nu->key = u;

    nu->outEdges.push_back(e);

    nodes[u] = nu;
    nodes[v] = nv;
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
int Graph::ford_fulkerson_max_flow(int s, int t) {
    
    Graph rGraph((Graph&) *this);
    std::vector<int> parent;
    parent.clear();
    parent.resize(m_edgesCount + 1);

    int max_flow = 0;
    int dist = 1;
    int path_flow = 0;
    while((path_flow = fattest_path()) > 0) {
        max_flow += path_flow;
    }

    return max_flow;

    while(dist > 0 && dist != INF) {

        // while there is a path, the dijkstra resulting is a
        path_flow = INF;
        dist = rGraph.dijkstraFattestPath(s, t, parent);
        std::cout << "dist: " << path_flow << std::endl;

//        for (int v = t; v != s && v != 0; v = parent[v]) {
//            int u = parent[v];
//            int w = rGraph.get(u, v);
//            path_flow = MIN(path_flow, w);
//            std::cout << u << " - " << path_flow << " -> " << v << std::endl;
//        }

        // update residual graph
        for (int v = t; v != s && v != 0; v = parent[v]) {
            int u = parent[v];
            rGraph.get(u, v) -= path_flow;
            rGraph.get(v, u) += path_flow;
            std::cout << u << " - " << rGraph.get(u, v) << " -> " << v << std::endl;
        }

        max_flow += path_flow;
    }

    return max_flow;
}

int Graph::dijkstraFattestPath(int s, int t, std::vector<int>& parent) {

    // mark visited vertices (auto release on scope exit)
    std::vector<bool> visited;
    std::vector<int> fat;
    visited.resize(m_verticesCount + 1);
    fat.resize(m_verticesCount + 1);

    // set all to false
    for (uint i = 0; i < m_verticesCount + 1; i++) {
        if (i != s) {
            visited[i] = false;
            fat[i] = 0;
        }
    }

    // source is parent of it self
    parent[s] = s;

    // init heap
    fat[s] = INF;
    HollowHeap<int, int> heap(INF, s);

    heap.comparison = [](int a, int b) -> bool {
        return a > b;
    };

    while(!heap.isEmpty()) {

        // get next vertice
        int u = heap.findminValue();
        heap.deleteMin();

        // cicle all neighbors
        for(auto& kvNeighbor : m_graph[u]) {

            int v = kvNeighbor.first;
            int w = kvNeighbor.second;
            int c = MIN(fat[u], w);

            // save minimum capacities
            if (fat[v] < c) {
                fat[v] = c;
                parent[v] = u;
                heap.insert(c, v);
            }
        }
    }

    return fat[t];
}

int Graph::fattest_path() {

    for(auto& kpNode : nodes) {
        if (kpNode.second != nullptr) {
            kpNode.second->parent_node = nullptr;
            kpNode.second->parent_edge = nullptr;
            kpNode.second->fat = 0;
        }
    }
    std::shared_ptr<Node> root = nodes[m_src];
    std::map<std::shared_ptr<Node>, int> Q;
    Q[root] = INF;
    root->fat = INF;

    while(!Q.empty()) {
        std::shared_ptr<Node> u = Q.cbegin()->first;
        Q.erase(u);

        for(std::shared_ptr<Edge> e : u->outEdges) {
            std::shared_ptr<Node> v = e->toNode;

            int minCap = (u->fat < e->capacity) ? u->fat : e->capacity;

            if (v->fat < minCap) {
                v->fat = minCap;

                Q[v] = v->fat;

                v->parent_node = u;
                v->parent_edge = e;
            }
        }

    }

    int path_flow = INF;

    {
        std::shared_ptr<Node> sink = nodes[m_sink];
        while(sink != root) {
            path_flow = MIN(path_flow, sink->fat);
            sink = sink->parent_node;
        }
    }

    {
        std::shared_ptr<Node> sink = nodes[m_sink];
        while(sink != root) {
            sink->parent_edge->capacity -= path_flow;
            sink = sink->parent_node;
        }
    }

    return path_flow;
}


int Graph::dijkstra(int s, int t, std::vector<int>& parent) {
    
    // mark visited vertices (auto release on scope exit)
    std::unique_ptr<bool[]> visited(new bool[m_verticesCount]);
    std::unique_ptr<int[]> distances(new int[m_verticesCount]);
    
    // set all to false
    for (uint i = 0; i < m_verticesCount; i++) {
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
