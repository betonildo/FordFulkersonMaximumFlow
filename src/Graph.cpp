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

    for (int i = 0; i <= m_verticesCount; i++) {
        nodes[i] = other.nodes[i];
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

    // Node pointers
    Node* ptrU = nodes[u] ? nodes[u] : new Node;
    ptrU->fat = 0;
    ptrU->key = u;
    ptrU->parent_edge = NULL;
    ptrU->parent_node = NULL;

    Node* ptrV = nodes[v] ? nodes[v] : new Node;
    ptrV->fat = 0;
    ptrV->key = v;
    ptrV->parent_edge = NULL;
    ptrV->parent_node = NULL;

    Edge* edge = new Edge;
    edge->capacity = w;
    edge->toNode = ptrV;

    ptrU->outEdges.push_back(edge);

    nodes[u] = ptrU;
    nodes[v] = ptrV;
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

//    // while there is a path, the dijkstra resulting is the fattest path
//    while((path_flow = rGraph.dijkstraFattestPath()) > 0) {
////        std::cout << "path_flow: " << path_flow << std::endl;
//        max_flow += path_flow;
//    }

    // while there is a path, the dijkstra resulting is the fattest path
    while((path_flow = rGraph.fatest_path()) > 0) {
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


int Graph::fatest_path() {

    for (auto& nkp : nodes) {
        Node* n = nkp.second;
        if (n == NULL) continue;
        n->parent_edge = NULL;
        n->parent_node = NULL;
        n->fat = 0;
    }

    Node* root = nodes[m_src];
    root->fat = INF;

    std::map<int, Node*> Q;

    Q[root->key] = root;

    while (Q.size() > 0) {
        Node* u = Q.begin()->second;
        Q.erase(u->key);

        for (Edge* e : u->outEdges) {
            Node* v = e->toNode;

            int minCap = (u->fat < e->capacity) ? u->fat : e->capacity;

            if (v->fat < minCap) {
                v->fat = minCap;

                Q[v->key] = v;

                v->parent_node = u;
                v->parent_edge = e;
            }
        }
    }

    int path_max = INF;

    for (Node* next = nodes[m_sink]; next != NULL; next = next->parent_node) {
        path_max = MIN(path_max, next->fat);
    }

    std::cout << "path flow: " << path_max << std::endl;
    std::cout << "path: ";
    for (Node* next = nodes[m_sink]; next != NULL && next->parent_edge != NULL; next = next->parent_node) {
        std::cout << next->key << " <- (" << next->parent_edge->capacity << ") - ";
        next->parent_edge->capacity -= path_max;
    }
    std::cout << root->key << std::endl;

    return path_max;
}
