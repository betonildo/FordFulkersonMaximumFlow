// C++ program for implementation of Ford Fulkerson algorithm
#include "read_dimacs.hpp"
#include <iostream>
#include <limits.h>
#include <string.h>
#include <map>
#include <vector>
#include <memory>
#include <queue>
using namespace std;

// Number of vertices in given graph
#define V 6

/* Returns true if there is a path from source 's' to sink 't' in
  residual graph. Also fills parent[] to store the path */
bool bfs(std::map<int, std::map<int, int> >& rGraph, int s, int t, std::vector<int>& parent)
{
    // Create a visited array and mark all vertices as not visited
    std::unique_ptr<bool[]> visited(new bool[rGraph.size()]);
    memset(visited.get(), 0, rGraph.size());

    // Create a queue, enqueue source vertex and mark source vertex
    // as visited
    queue <int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    // Standard BFS Loop
    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (int v=0; v<V; v++)
        {
            if (visited[v]==false && rGraph[u][v] > 0)
            {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }

    // If we reached sink in BFS starting from source, then return
    // true, else false
    return (visited[t] == true);
}

// Returns the maximum flow from s to t in the given graph
int fordFulkerson(std::map<int, std::map<int, int> >& rGraph, int s, int t)
{
    int u, v;

    // Create a residual graph and fill the residual graph with
    // given capacities in the original graph as residual capacities
    // in residual graph

    std::vector<int> parent;  // This array is filled by BFS and to store path
    parent.clear();
    parent.reserve(rGraph.size());

    int max_flow = 0;  // There is no flow initially

    // Augment the flow while tere is path from source to sink
    while (bfs(rGraph, s, t, parent))
    {
        // Find minimum residual capacity of the edges along the
        // path filled by BFS. Or we can say find the maximum flow
        // through the path found.
        int path_flow = INT_MAX;
        for (v=t; v!=s; v=parent[v])
        {
            u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }

        // update residual capacities of the edges and reverse edges
        // along the path
        for (v=t; v != s; v=parent[v])
        {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }

        // Add path flow to overall flow
        max_flow += path_flow;
    }

    // Return the overall flow
    return max_flow;
}

// Driver program to test above functions
int main(int argc, char** argv)
{
    uint s = atoi(argv[1]);
    uint t = atoi(argv[2]);

    std::map<int, std::map<int, int> > g;

    if (argc == 4) read_dimacs(fopen(argv[3], "r"), g);
    else read_dimacs(stdin, g);

    cout << "The maximum possible flow is " << fordFulkerson(g, s, t);

    return 0;
}