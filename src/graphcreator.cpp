#include <limits>
#include <sealib/graphcreator.h>

Sealib::Basicgraph Sealib::GraphCreator::createGraphFromAdjacencyMatrix(unsigned int **adjMatrix,
                                                         unsigned int order) {
    std::vector<Node> nodes(order);

    for (unsigned int i = 0; i < order; i++) {
        unsigned int deg = 0;

        for (unsigned int j = 0; j < order; j++) {
            deg += adjMatrix[i][j];
        }

        std::vector<Adjacency> adj(deg);

        int idx = 0;
        for (unsigned int j = 0; j < order; j++) {
            for (unsigned int k = 0; k < adjMatrix[i][j]; k++) {
                adj[idx++] = Adjacency(j);
            }
        }
        nodes[i] = Node(adj);
    }

    for (unsigned int i = 0; i < order; i++) {
        const unsigned int deg = nodes[i].getDegree();
        const std::vector<Adjacency> &adj_arr = nodes[i].getAdj();

        for (unsigned int j = 0; j < deg; j++) {
            if (adj_arr[j].crossIndex == std::numeric_limits<unsigned int>::max()) {
                unsigned int v = adj_arr[j].vertex;
                const std::vector<Adjacency> &_adj_arr = nodes[v].getAdj();
                const unsigned int _deg = nodes[v].getDegree();

                for (unsigned int _j = 0; _j < _deg; _j++) {
                    if (_adj_arr[_j].crossIndex == std::numeric_limits<unsigned int>::max() &&
                        _adj_arr[_j].vertex == i) {
                        nodes[v].setCrossIndex(_j, j);
                        nodes[i].setCrossIndex(j, _j);
                        break;
                    }
                }
            }
        }
    }

    return Basicgraph(nodes);
}
