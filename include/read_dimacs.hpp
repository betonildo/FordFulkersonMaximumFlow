#ifndef READ_DIMACS_H
#define READ_DIMACS_H

#define BUFSIZE 1024

#include <cstring>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

template<typename Graph>
void read_dimacs(FILE* stream, Graph& g) {
    if (stream == NULL) {
        std::cout << "Error occured opening the file" << std::endl;
        exit(-1);
    }

    char buffer[BUFSIZE];
    unsigned int n = 0, m = 0;
    memset(buffer, 0, BUFSIZE);
    std::string line = fgets(buffer, BUFSIZE, stream);
    while (line.substr(0, 4) != "p sp" && line.substr(0, 5) != "p max") {
        memset(buffer, 0, BUFSIZE);
        line = fgets(buffer, BUFSIZE, stream);
        // std::cout << "line content: " << line << std::endl;
    }
    
    unsigned int size = line.length();
    
    memset(buffer, 0, BUFSIZE);
    memcpy(buffer, line.c_str(), size);
    char sp[3];
    char s;
    sscanf(buffer, "%c %s %d %d", &s, sp, &n, &m);
    // g.setSize(n);

    unsigned int u, v, w;
    unsigned int i = 0;

    while (i < m) {
        
        line = fgets(buffer, BUFSIZE, stream);
        if (line.substr(0,2) == "a ") {
            sscanf(buffer, "%c %d %d %d", &s, &u, &v, &w);
            // std::cout << s << " " << u << " " << v << " " << w << std::endl;
            // processar arco (u,v) com peso w
            g.set(u, v, w);
            i++;
        }
    }
}

#endif /*READ_DIMACS_H*/