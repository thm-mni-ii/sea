#include <sealib/graph/graphcreator.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <set>

using Sealib::DirectedGraph;
using Sealib::UndirectedGraph;
using Sealib::CompactGraph;
using Sealib::GraphCreator;

UndirectedGraph *Sealib::GraphCreator::createGraphPointerFromAdjacencyMatrix(
    uint32_t **adjMatrix, uint32_t order) {
    std::vector<Node> nodes(order);

    for (uint32_t i = 0; i < order; i++) {
        uint32_t deg = 0;

        for (uint32_t j = 0; j < order; j++) {
            deg += adjMatrix[i][j];
        }

        std::vector<Adjacency> adj(deg);

        uint32_t idx = 0;
        for (uint32_t j = 0; j < order; j++) {
            for (uint32_t k = 0; k < adjMatrix[i][j]; k++) {
                adj[idx++] = Adjacency(j);
            }
        }
        nodes[i] = Node(adj);
    }
    for (uint32_t i = 0; i < order; i++) {
        const uint32_t deg = nodes[i].getDegree();
        const std::vector<Adjacency> &adj_arr = nodes[i].getAdj();

        for (uint32_t j = 0; j < deg; j++) {
            if (adj_arr[j].crossIndex == std::numeric_limits<uint32_t>::max()) {
                uint32_t v = adj_arr[j].vertex;
                const std::vector<Adjacency> &_adj_arr = nodes[v].getAdj();
                const uint32_t _deg = nodes[v].getDegree();

                for (uint32_t _j = 0; _j < _deg; _j++) {
                    if (_adj_arr[_j].crossIndex ==
                            std::numeric_limits<uint32_t>::max() &&
                        _adj_arr[_j].vertex == i) {
                        nodes[v].setCrossIndex(_j, j);
                        nodes[i].setCrossIndex(j, _j);
                        break;
                    }
                }
            }
        }
    }
    return new UndirectedGraph(nodes);
}

UndirectedGraph GraphCreator::createGraphFromAdjacencyMatrix(
    uint32_t **adjMatrix, uint32_t order) {
    return *createGraphPointerFromAdjacencyMatrix(adjMatrix, order);
}

std::shared_ptr<UndirectedGraph>
GraphCreator::createSharedGraphFromAdjacencyMatrix(uint32_t **adjMatrix,
                                                   uint32_t order) {
    return std::shared_ptr<UndirectedGraph>(
        createGraphPointerFromAdjacencyMatrix(adjMatrix, order));
}

std::unique_ptr<Sealib::UndirectedGraph>
Sealib::GraphCreator::generateRandomBipartiteUndirectedGraph(uint32_t order1,
                                                             uint32_t order2,
                                                             double p,
                                                             uint32_t seed) {
    std::unique_ptr<Sealib::UndirectedGraph> graph(
        new Sealib::UndirectedGraph(order1 + order2));

    std::mt19937_64 _rng(seed);
    std::uniform_real_distribution<double> unif(0.0, 1.0);

    for (uint32_t n1 = 0; n1 < order1; n1++) {
        for (uint32_t n2 = order1; n2 < order2; n2++) {
            if (unif(_rng) < p) {
                Sealib::Node &node1 = graph->getNode(n1);
                Sealib::Node &node2 = graph->getNode(n2);

                uint32_t n1idx = node1.getDegree();
                uint32_t n2idx = node2.getDegree();

                node1.addAdjacency(n2);
                node1.setCrossIndex(n1idx, n2idx);

                node2.addAdjacency(n1);
                node2.setCrossIndex(n2idx, n1idx);
            }
        }
    }

    return graph;
}

static std::random_device rng;

Sealib::DirectedGraph GraphCreator::createRandomImbalanced(uint32_t order) {
    std::vector<Node> n(order);
    std::uniform_int_distribution<uint32_t> rnd(0, order - 1);
    std::uniform_int_distribution<uint32_t> dist1(order * order,
                                                  2 * order * order);
    std::uniform_int_distribution<uint32_t> dist2(
        0, static_cast<uint32_t>(ceil(log2(order))));
    std::set<uint32_t> big;
    for (uint32_t a = 0; a < ceil(order / (2 * log2(order))); a++)
        big.insert(rnd(rng));
    for (uint32_t a = 0; a < order; a++) {
        uint32_t deg;
        if (big.find(a) == big.end()) {
            deg = dist2(rng);
        } else {
            deg = dist1(rng);
        }
        std::vector<Adjacency> ad(deg);
        for (uint32_t b = 0; b < deg; b++) {
            ad[b] = Adjacency(rnd(rng));
        }
        n[a] = Node(ad);
    }
    return DirectedGraph(n);
}

Sealib::DirectedGraph Sealib::GraphCreator::createRandomKRegularGraph(
    uint32_t order, uint32_t degreePerNode) {
    std::uniform_int_distribution<uint32_t> rnd(0, order - 1);
    std::vector<Node> n(order);
    for (uint32_t a = 0; a < order; a++) {
        uint32_t ai = degreePerNode;
        std::vector<Adjacency> ad(ai);
        for (uint32_t b = 0; b < ai; b++) {
            ad[b] = Adjacency(rnd(rng));
        }
        n[a] = Node(ad);
    }
    return DirectedGraph(n);
}

Sealib::DirectedGraph Sealib::GraphCreator::createRandomGenerated(
    uint32_t order) {
    std::vector<Node> n(order);
    std::uniform_int_distribution<uint32_t> rnd(0, order - 1);
    for (uint32_t a = 0; a < order; a++) {
        uint32_t deg = rnd(rng);
        std::vector<Adjacency> ad(deg);
        for (uint32_t b = 0; b < deg; b++) {
            ad[b] = Adjacency(rnd(rng));
        }
        n[a] = Node(ad);
    }
    return DirectedGraph(n);
}

std::pair<UndirectedGraph *, uint32_t> GraphCreator::createRandomUndirected(
    uint32_t order, uint32_t approxDegree) {
    UndirectedGraph *g = new UndirectedGraph(order);
    std::uniform_int_distribution<uint32_t> dist(0, order - 1);
    uint64_t sum = 0;
    for (uint32_t a = 0; a < order; a++) {
        while (g->getNodeDegree(a) < approxDegree) {
            uint32_t b = dist(rng);
            Node &n1 = g->getNode(a), &n2 = g->getNode(b);
            uint32_t i1 = g->getNodeDegree(a), i2 = g->getNodeDegree(b);
            n1.addAdjacency(b);
            n1.setCrossIndex(i1, i2);
            n2.addAdjacency(a);
            n2.setCrossIndex(i2, i1);
            sum += 2;
        }
    }
    return {g, sum};
}

UndirectedGraph GraphCreator::createWindmill(uint32_t order, uint32_t count) {
    order--;
    uint32_t n = order * count + 1;
    UndirectedGraph g(n);
    for (uint32_t a = 0; a < count; a++) {
        // a = no. complete graphs
        for (uint32_t b = a * order; b < (a + 1) * order - 1; b++) {
            // b = no. source nodes
            for (uint32_t c = b + 1; c < (a + 1) * order; c++) {
                // c = no. dest. nodes
                uint32_t i1 = g.getNodeDegree(b), i2 = g.getNodeDegree(c);
                g.getNode(b).addAdjacency(c);
                g.getNode(b).setCrossIndex(i1, i2);
                g.getNode(c).addAdjacency(b);
                g.getNode(c).setCrossIndex(i2, i1);
            }
        }
    }
    for (uint a = 0; a < n - 1; a++) {
        uint32_t i1 = g.getNodeDegree(n - 1), i2 = g.getNodeDegree(a);
        g.getNode(n - 1).addAdjacency(a);
        g.getNode(n - 1).setCrossIndex(i1, i2);
        g.getNode(a).addAdjacency(n - 1);
        g.getNode(a).setCrossIndex(i2, i1);
    }
    return g;
}

static uint32_t *generateRawGilbertGraph(uint32_t order, double p,
                                         std::mt19937_64 *gen) {
    uint32_t size = 0;
    uint32_t *edgeArray = new uint32_t[order];
    std::binomial_distribution<uint32_t> dist(order - 1, p);
    for (uint32_t i = 0; i < order; ++i) {
        uint32_t edges = dist(*gen);
        dist.reset();
        size += edges;
        edgeArray[i] = edges;
    }
    uint32_t *graph = new uint32_t[order + size + 2];

    graph[0] = order;
    graph[order + 1] = size;

    uint32_t lastPosition = order + 2;
    for (uint32_t i = 1; i <= order; ++i) {
        if (edgeArray[i - 1] == 0) {
            graph[i] = i;
        } else {
            graph[i] = lastPosition;
            lastPosition += edgeArray[i - 1];
        }
    }
    bool initialBit = 0;
    if (p > 0.5) {
        initialBit = 1;
    }
    std::vector<bool> bitVector(order, initialBit);
    std::random_device rndDev;
    std::uniform_int_distribution<uint32_t> dist2(0, order - 1);
    for (uint32_t i = 0; i < order; ++i) {
        uint32_t numBitsSet = 0;
        // a[i] = number of edges from i
        uint32_t bitsToSet = edgeArray[i];
        if (initialBit == 1) {
            bitsToSet = order - bitsToSet;
        }
        while (edgeArray[i] > numBitsSet) {
            uint32_t rnd = dist2(rndDev);
            if (bitVector[rnd] == initialBit && rnd != i) {
                bitVector[rnd] = !initialBit;
                numBitsSet += 1;
            }
        }
        uint32_t pos = graph[i + 1];
        for (uint32_t j = 0; j < order; ++j) {
            if (bitVector[j] == !initialBit) {
                graph[pos++] = j + 1;
                bitVector[j] = initialBit;
            }
        }
    }
    delete[] edgeArray;
    return graph;
}

uint *GraphCreator::fastGraphGeneration(uint n, uint mPern) {
    uint M = n * mPern;
    uint N = n + M + 2;

    uint32_t *A = new uint32_t[N];
    A[0] = n;
    A[n + 1] = M;
    std::random_device rd;
    std::mt19937_64 mt(rd());
    mt.seed(std::mt19937_64::default_seed);

    A[1] = n + 2;

    for (uint v = 1; v <= n; ++v) {
        // First: Create Pointer!
        if (v != n) {
            A[v + 1] = A[v] + mPern;
        }
        std::set<uint> generated;
        for (uint p = A[v]; p < N && (v == n || p < A[v + 1]); p++) {
            std::uniform_int_distribution<uint> dist(1, n);
            uint rn = dist(mt);
            if (generated.find(rn) != generated.end()) {
                --p;
            } else {
                A[p] = rn;
                generated.insert(rn);
            }
        }
        generated.clear();
        if (v != n) {
            std::sort(A + (A[v]), A + (A[v + 1]));
        }
    }
    std::sort(A + (A[n]), A + N);
    return A;
}

CompactGraph GraphCreator::generateGilbertGraph(uint32_t order, double p,
                                                std::mt19937_64 *gen) {
    return CompactGraph(generateRawGilbertGraph(order, p, gen));
}
