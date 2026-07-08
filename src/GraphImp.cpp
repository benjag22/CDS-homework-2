#include "AdjacencyList.cpp"
#include "sdsl/pemb.hpp"

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
