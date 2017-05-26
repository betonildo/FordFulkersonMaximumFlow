#include "definitions.hpp"
#include "Graph.hpp"
#include "read_dimacs.hpp"

int main(int argc, char** argv) {

    Graph g;
    if (argc == 2) read_dimacs(fopen(argv[1], "r"), g);
    else read_dimacs(stdin, g);
    std::cout << "max flow: " << g.ford_fulkerson_max_flow() << std::endl;

    return 0;
}
