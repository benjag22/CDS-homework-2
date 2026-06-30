#pragma once

#include <sdsl/pemb.hpp>
#include "AdjacencyList.cpp"

enum class Representation {
    ADJACENCY_LIST,
    K2_TREE,
    TURAN
};
class GraphImp {
public:

private:
    sdsl::pemb<> graph;
    AdjacencyList adjList;

};
