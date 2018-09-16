#include <sealib/graphcreator.h>
#include <limits>

Sealib::Basicgraph Sealib::GraphCreator::createGraphFromAdjacencyMatrix(
    unsigned int **adjMatrix,
    unsigned int order) {
    std::vector<Node> nodes;

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
        nodes.emplace_back(adj);
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

static std::random_device rng;

Sealib::Basicgraph *Sealib::GraphCreator::createRandomFixed(unsigned int order,
                                                            unsigned int degreePerNode) {
    std::uniform_int_distribution<unsigned int> rnd(0, order - 1);
    std::vector<Node> n(order);
    for (unsigned int a = 0; a < order; a++) {
        unsigned int ai = degreePerNode;
        std::vector<Adjacency> ad(ai);
        for (unsigned int b = 0; b < ai; b++) {
            ad[b] = Adjacency(rnd(rng));
        }
        n[a] = Node(ad);
    }
    return new Basicgraph(n);
}

std::unique_ptr<Sealib::BasicGraph> Sealib::GraphCreator::generateRandomBipartiteBasicGraph(unsigned int order1,
                                                                                            unsigned int order2,
                                                                                            double p,
                                                                                            unsigned int seed) {
    std::unique_ptr<Sealib::BasicGraph> graph(new Sealib::BasicGraph(order1+order2));

    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<double> unif(0.0, 1.0);

    for (unsigned int n1 = 0; n1 < order1; n1++) {
        for (unsigned int n2 = order1; n2 < order2; n2++) {
            if (unif(rng) < p) {
                Sealib::Node &node1 = graph->getNode(n1);
                Sealib::Node &node2 = graph->getNode(n2);

                unsigned int n1idx = node1.getDegree();
                unsigned int n2idx = node2.getDegree();

                node1.addAdjacency(n2);
                node1.setCrossIndex(n1idx, n2idx);

                node2.addAdjacency(n1);
                node2.setCrossIndex(n2idx, n1idx);
            }
        }
    }

    return graph;
}
