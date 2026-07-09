#include "adj_list_graph.hpp"
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
    AdjListGraph adjList;
};
