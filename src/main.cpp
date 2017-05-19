#include "definitions.hpp"
#include "Graph.hpp"
#include "read_dimacs.hpp"

int main(int argc, char** argv) {

    if (argc < 3) {
        std::cout << "usage: " << argv[0] << " s t < graph.txt" << std::endl;
        return -1;
    }

    uint s = atoi(argv[1]);
    uint t = atoi(argv[2]);

    Graph g;

    if (argc == 4) read_dimacs(fopen(argv[3], "r"), g);
    else read_dimacs(stdin, g);
    
    bool has_path = g.breadth_first_transversal(s, t);

    std::cout << "args: ";
    for (int i = 1; i < argc; i++) {
        std::cout << argv[i] << " ";
    }
    std::cout << std::endl;
    std::cout << (has_path ? "true" : "false") << " that has path between " << s << " and " << t << std::endl;
    std::cout << "max flow: " << g.ford_fulkerson_max_flow(s, t) << std::endl;

    return 0;
}
