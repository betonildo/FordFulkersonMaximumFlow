mkdir build
g++ -Wall -ggdb3 -std=c++11 src/Graph.cpp src/main.cpp -o build/FordFulkersonMaximumFlow -I"include/" -lm
