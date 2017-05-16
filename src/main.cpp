//#include "definitions.hpp"
//#include "Graph.hpp"
//#include "read_dimacs.hpp"

#include "HollowHeap.hpp"

int main(int argc, char** argv) {

//    if (argc < 3) {
//        std::cout << "usage: " << argv[0] << " s t < graph.txt" << std::endl;
//        return -1;
//    }

//    uint s = atoi(argv[1]);
//    uint t = atoi(argv[2]);

//    Graph g;

//    if (argc == 4) read_dimacs(fopen(argv[3], "r"), g);
//    else read_dimacs(stdin, g);
    

//    bool has_path = g.breadth_first_transversal(s, t);

//    std::cout << std::endl << has_path << " that has path between " << s << " and " << t << std::endl;

    HollowHeap<U32, U32> hh(1, 0);



    hh.insert(1000, 1000);
    hh.insert(123, 123);
    hh.insert(12, 12);
    hh.deleteMin();
    hh.deleteMin();
    hh.deleteMin();
    hh.deleteMin();

    std::cout << "Minimum Value: " << hh.findminValue() << std::endl;
    return 0;
}
