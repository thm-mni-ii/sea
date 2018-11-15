#include <sealib/graphcreator.h>
#include <stdlib.h>
#include <iostream>
#include <limits>
#include <set>

using Sealib::BasicGraph;
using Sealib::GraphCreator;

BasicGraph GraphCreator::createGraphFromAdjacencyMatrix(
    unsigned int **adjMatrix, unsigned int order) {
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
            if (adj_arr[j].crossIndex ==
                std::numeric_limits<unsigned int>::max()) {
                unsigned int v = adj_arr[j].vertex;
                const std::vector<Adjacency> &_adj_arr = nodes[v].getAdj();
                const unsigned int _deg = nodes[v].getDegree();

                for (unsigned int _j = 0; _j < _deg; _j++) {
                    if (_adj_arr[_j].crossIndex ==
                            std::numeric_limits<unsigned int>::max() &&
                        _adj_arr[_j].vertex == i) {
                        nodes[v].setCrossIndex(_j, j);
                        nodes[i].setCrossIndex(j, _j);
                        break;
                    }
                }
            }
        }
    }
    return BasicGraph(nodes);
}

std::shared_ptr<BasicGraph> GraphCreator::createSharedGraphFromAdjacencyMatrix(
    unsigned int **adjMatrix, unsigned int order) {
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
            if (adj_arr[j].crossIndex ==
                std::numeric_limits<unsigned int>::max()) {
                unsigned int v = adj_arr[j].vertex;
                const std::vector<Adjacency> &_adj_arr = nodes[v].getAdj();
                const unsigned int _deg = nodes[v].getDegree();

                for (unsigned int _j = 0; _j < _deg; _j++) {
                    if (_adj_arr[_j].crossIndex ==
                            std::numeric_limits<unsigned int>::max() &&
                        _adj_arr[_j].vertex == i) {
                        nodes[v].setCrossIndex(_j, j);
                        nodes[i].setCrossIndex(j, _j);
                        break;
                    }
                }
            }
        }
    }
    return std::shared_ptr<BasicGraph>(new BasicGraph(nodes));
}

BasicGraph *Sealib::GraphCreator::createGraphPointerFromAdjacencyMatrix(
    unsigned int **adjMatrix, unsigned int order) {
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
            if (adj_arr[j].crossIndex ==
                std::numeric_limits<unsigned int>::max()) {
                unsigned int v = adj_arr[j].vertex;
                const std::vector<Adjacency> &_adj_arr = nodes[v].getAdj();
                const unsigned int _deg = nodes[v].getDegree();

                for (unsigned int _j = 0; _j < _deg; _j++) {
                    if (_adj_arr[_j].crossIndex ==
                            std::numeric_limits<unsigned int>::max() &&
                        _adj_arr[_j].vertex == i) {
                        nodes[v].setCrossIndex(_j, j);
                        nodes[i].setCrossIndex(j, _j);
                        break;
                    }
                }
            }
        }
    }
    return new BasicGraph(nodes);
}

static std::random_device rng;

BasicGraph *GraphCreator::createRandomImbalanced(unsigned int order) {
    std::vector<Node> n(order);
    std::uniform_int_distribution<unsigned int> rnd(0, order - 1);
    std::uniform_int_distribution<unsigned int> dist1(order * order,
                                                      2 * order * order);
    std::uniform_int_distribution<unsigned int> dist2(
        0, static_cast<unsigned int>(ceil(log2(order))));
    std::set<unsigned> big;
    for (unsigned a = 0; a < ceil(order / (2 * log2(order))); a++)
        big.insert(rnd(rng));
    for (unsigned int a = 0; a < order; a++) {
        unsigned int deg;
        if (big.find(a) == big.end()) {
            deg = dist2(rng);
        } else {
            deg = dist1(rng);
        }
        std::vector<Adjacency> ad(deg);
        for (unsigned int b = 0; b < deg; b++) {
            ad[b] = Adjacency(rnd(rng));
        }
        n[a] = Node(ad);
    }
    return new BasicGraph(n);
}

std::unique_ptr<Sealib::BasicGraph>
Sealib::GraphCreator::generateRandomBipartiteBasicGraph(unsigned int order1,
                                                        unsigned int order2,
                                                        double p,
                                                        unsigned int seed) {
    std::unique_ptr<Sealib::BasicGraph> graph(
        new Sealib::BasicGraph(order1 + order2));

    std::mt19937_64 _rng(seed);
    std::uniform_real_distribution<double> unif(0.0, 1.0);

    for (unsigned int n1 = 0; n1 < order1; n1++) {
        for (unsigned int n2 = order1; n2 < order2; n2++) {
            if (unif(_rng) < p) {
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

Sealib::BasicGraph *Sealib::GraphCreator::createRandomFixed(
    unsigned int order, unsigned int degreePerNode) {
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
    return new BasicGraph(n);
}

Sealib::BasicGraph *Sealib::GraphCreator::createRandomGenerated(
    unsigned int order) {
    std::vector<Node> n(order);
    std::uniform_int_distribution<unsigned int> rnd(0, order - 1);
    for (unsigned int a = 0; a < order; a++) {
        unsigned int deg = rnd(rng);
        std::vector<Adjacency> ad(deg);
        for (unsigned int b = 0; b < deg; b++) {
            ad[b] = Adjacency(rnd(rng));
        }
        n[a] = Node(ad);
    }
    return new BasicGraph(n);
}

std::pair<BasicGraph *, unsigned int> GraphCreator::createRandomUndirected(
    unsigned int order, unsigned int approxDegree) {
    BasicGraph *g = new BasicGraph(order);
    std::uniform_int_distribution<unsigned int> dist(0, order - 1);
    unsigned long sum = 0;
    for (unsigned int a = 0; a < order; a++) {
        while (g->getNodeDegree(a) < approxDegree) {
            unsigned int b = dist(rng);
            Node &n1 = g->getNode(a), &n2 = g->getNode(b);
            unsigned int i1 = g->getNodeDegree(a), i2 = g->getNodeDegree(b);
            n1.addAdjacency(b);
            n1.setCrossIndex(i1, i2);
            n2.addAdjacency(a);
            n2.setCrossIndex(i2, i1);
            sum += 2;
        }
    }
    return {g, sum};
}

BasicGraph *GraphCreator::createWindmill(unsigned int order,
                                         unsigned int count) {
    order--;
    unsigned int n = order * count + 1;
    BasicGraph *g = new BasicGraph(n);
    for (unsigned int a = 0; a < count; a++) {
        // a = no. complete graphs
        for (unsigned int b = a * order; b < (a + 1) * order - 1; b++) {
            // b = no. source nodes
            for (unsigned int c = b + 1; c < (a + 1) * order; c++) {
                // c = no. dest. nodes
                unsigned int i1 = g->getNodeDegree(b), i2 = g->getNodeDegree(c);
                g->getNode(b).addAdjacency(c);
                g->getNode(b).setCrossIndex(i1, i2);
                g->getNode(c).addAdjacency(b);
                g->getNode(c).setCrossIndex(i2, i1);
            }
        }
    }
    for (uint a = 0; a < n - 1; a++) {
        unsigned int i1 = g->getNodeDegree(n - 1), i2 = g->getNodeDegree(a);
        g->getNode(n - 1).addAdjacency(a);
        g->getNode(n - 1).setCrossIndex(i1, i2);
        g->getNode(a).addAdjacency(n - 1);
        g->getNode(a).setCrossIndex(i2, i1);
    }
    return g;
}
