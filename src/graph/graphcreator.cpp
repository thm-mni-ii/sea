#include <sealib/graph/graphcreator.h>
#include <stdlib.h>
#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <limits>
#include <random>
#include <set>

using Sealib::CompactGraph;
using Sealib::DirectedGraph;
using Sealib::GraphCreator;
using Sealib::UndirectedGraph;

UndirectedGraph *Sealib::GraphCreator::createPointerFromAdjacencyMatrix(
    uint64_t **adjMatrix, uint64_t order) {
    std::vector<ExtendedNode> nodes;

    for (uint64_t i = 0; i < order; i++) {
        std::vector<std::pair<uint64_t, uint64_t>> adj;

        for (uint64_t j = 0; j < order; j++) {
            for (uint64_t k = 0; k < adjMatrix[i][j]; k++)
                adj.push_back({j, INVALID});
        }

        nodes.push_back(ExtendedNode(adj));
    }
    for (uint64_t i = 0; i < order; i++) {
        const uint64_t deg1 = nodes[i].getDegree();
        std::vector<std::pair<uint64_t, uint64_t>> &n1 = nodes[i].getAdj();

        for (uint64_t k1 = 0; k1 < deg1; k1++) {
            if (n1[k1].second == INVALID) {
                uint64_t v = n1[k1].first;
                std::vector<std::pair<uint64_t, uint64_t>> &n2 =
                    nodes[v].getAdj();
                const uint64_t deg2 = nodes[v].getDegree();

                for (uint64_t k2 = 0; k2 < deg2; k2++) {
                    if (n2[k2].second == INVALID && n2[k2].first == i) {
                        n2[k2].second = k1;
                        n1[k1].second = k2;
                        break;
                    }
                }
            }
        }
    }
    return new UndirectedGraph(nodes);
}

UndirectedGraph GraphCreator::createFromAdjacencyMatrix(uint64_t **adjMatrix,
                                                        uint64_t order) {
    return *createPointerFromAdjacencyMatrix(adjMatrix, order);
}

std::shared_ptr<UndirectedGraph>
GraphCreator::createSharedPointerFromAdjacencyMatrix(uint64_t **adjMatrix,
                                                     uint64_t order) {
    return std::shared_ptr<UndirectedGraph>(
        createPointerFromAdjacencyMatrix(adjMatrix, order));
}

std::unique_ptr<Sealib::UndirectedGraph> Sealib::GraphCreator::randomBipartite(
    uint64_t order1, uint64_t order2, double p, uint64_t seed) {
    std::unique_ptr<Sealib::UndirectedGraph> graph(
        new Sealib::UndirectedGraph(order1 + order2));

    std::mt19937_64 _rng(seed);
    std::uniform_real_distribution<double> unif(0.0, 1.0);

    for (uint64_t n1 = 0; n1 < order1; n1++) {
        for (uint64_t n2 = order1; n2 < order2; n2++) {
            if (unif(_rng) < p) {
                ExtendedNode &node1 = graph->getNode(n1);
                ExtendedNode &node2 = graph->getNode(n2);

                uint64_t n1idx = node1.getDegree();
                uint64_t n2idx = node2.getDegree();

                node1.addAdjacency({n2, n2idx});

                node2.addAdjacency({n1, n1idx});
            }
        }
    }

    return graph;
}

static std::random_device rng;

Sealib::DirectedGraph GraphCreator::imbalanced(uint64_t order) {
    DirectedGraph g(order);
    std::uniform_int_distribution<uint64_t> distN(0, order - 1);
    std::vector<bool> big(order);
    for (uint64_t a = 0; a < log2(order); a++) {
        big[a] = distN(rng);
    }
    for (uint64_t a = 0; a < order; a++) {
        uint64_t deg;
        if (big[a]) {
            deg = 4 * order;
        } else {
            deg = static_cast<uint64_t>(ceil(log2(order)));
        }
        for (uint64_t b = 0; b < deg; b++) {
            g.getNode(a).addAdjacency(distN(rng));
        }
    }
    return g;
}

Sealib::DirectedGraph Sealib::GraphCreator::kOutdegree(uint64_t order,
                                                       uint64_t degreePerNode) {
    std::uniform_int_distribution<uint64_t> rnd(0, order - 1);
    std::vector<SimpleNode> n(order);
    for (uint64_t a = 0; a < order; a++) {
        std::vector<uint64_t> ad(degreePerNode);
        for (uint64_t b = 0; b < degreePerNode; b++) {
            ad[b] = rnd(rng);
        }
        n[a] = SimpleNode(ad);
    }
    return DirectedGraph(n);
}

Sealib::DirectedGraph Sealib::GraphCreator::sparseDirected(uint64_t order) {
    std::vector<SimpleNode> n(order);
    std::uniform_int_distribution<uint64_t> nR(0, order - 1);
    std::uniform_int_distribution<uint64_t> degR(
        0, static_cast<uint64_t>(log2(order)));
    for (uint64_t a = 0; a < order; a++) {
        uint64_t deg = degR(rng);
        std::vector<uint64_t> ad(deg);
        for (uint64_t b = 0; b < deg; b++) {
            ad[b] = nR(rng);
        }
        n[a] = SimpleNode(ad);
    }
    return DirectedGraph(n);
}

UndirectedGraph GraphCreator::sparseUndirected(uint64_t order) {
    UndirectedGraph g(order);
    std::uniform_int_distribution<uint64_t> nR(0, order - 1);
    std::uniform_int_distribution<uint64_t> degR(
        0, static_cast<uint64_t>(log2(order)));
    for (uint64_t a = 0; a < order; a++) {
        for (uint64_t c = 0; c < degR(rng); c++) {
            uint64_t b = nR(rng);
            ExtendedNode &n1 = g.getNode(a), &n2 = g.getNode(b);
            uint64_t i1 = g.deg(a), i2 = g.deg(b);
            n1.addAdjacency({b, i2});
            n2.addAdjacency({a, i1});
        }
    }
    return g;
}

UndirectedGraph GraphCreator::kRegular(uint64_t order, uint64_t degreePerNode) {
    assert("Cannot create a k-regular graph with an odd number of vertices" &&
           order % 2 == 0);
    UndirectedGraph g(order);
    for (uint64_t k = 0; k < degreePerNode; k++) {
        std::vector<uint64_t> u;
        for (uint64_t a = 0; a < order; a++) {
            u.emplace_back(a);
        }
        std::shuffle(u.begin(), u.end(), rng);
        while (!u.empty()) {
            uint64_t u1 = u.back();
            u.pop_back();
            uint64_t u2 = u.back();
            u.pop_back();
            ExtendedNode &n1 = g.getNode(u1);
            ExtendedNode &n2 = g.getNode(u2);
            n1.addAdjacency({u2, k});
            n2.addAdjacency({u1, k});
        }
    }
    return g;
}

UndirectedGraph GraphCreator::windmill(uint64_t order, uint64_t count) {
    order--;
    uint64_t n = order * count + 1;
    UndirectedGraph g(n);
    for (uint64_t a = 0; a < count; a++) {
        // a = no. complete graphs
        for (uint64_t b = a * order; b < (a + 1) * order - 1; b++) {
            // b = no. source nodes
            for (uint64_t c = b + 1; c < (a + 1) * order; c++) {
                // c = no. dest. nodes
                uint64_t i1 = g.deg(b), i2 = g.deg(c);
                g.getNode(b).addAdjacency({c, i2});
                g.getNode(c).addAdjacency({b, i1});
            }
        }
    }
    for (uint64_t a = 0; a < n - 1; a++) {
        uint64_t i1 = g.deg(n - 1), i2 = g.deg(a);
        g.getNode(n - 1).addAdjacency({a, i2});
        g.getNode(a).addAdjacency({n - 1, i1});
    }
    return g;
}

DirectedGraph GraphCreator::transpose(DirectedGraph const &g) {
    DirectedGraph t(g.getOrder());
    for (uint64_t u = 0; u < g.getOrder(); u++) {
        for (uint64_t k = 0; k < g.deg(u); k++) {
            uint64_t v = g.head(u, k);
            t.getNode(v).addAdjacency(u);
        }
    }
    return t;
}

UndirectedGraph GraphCreator::triangulated(uint64_t order) {
    assert("Cannot create a triangle graph with less than 3 vertices" &&
           order >= 3);
    std::vector<ExtendedNode> nodes;
    nodes.emplace_back(ExtendedNode({{1, 0}, {2, 0}}));
    nodes.emplace_back(ExtendedNode({{0, 0}, {2, 1}}));
    nodes.emplace_back(ExtendedNode({{0, 1}, {1, 1}}));
    uint64_t n = 3;
    while (n < order) {
        uint64_t d = nodes.back().getDegree();
        nodes[n - 2].addAdjacency({n, 0});
        nodes[n - 1].addAdjacency({n, 1});
        nodes.emplace_back(ExtendedNode({{n - 2, d - 1}, {n - 1, d - 1}}));
        n++;
    }
    return UndirectedGraph(nodes);
}

UndirectedGraph GraphCreator::cycle(uint64_t order, uint64_t chords) {
    std::vector<ExtendedNode> nodes;
    uint64_t n = 0;
    while (nodes.size() < order) {
        nodes.emplace_back(ExtendedNode({{n == 0 ? order - 1 : n - 1, 1},
                                         {n == order - 1 ? 0 : n + 1, 0}}));
        n++;
    }
    if (chords > 0) {
        uint64_t l = n / chords, u = 0;
        while (u + l / 2 < n) {
            nodes[u].addAdjacency({u + l / 2, 2});
            nodes[u + l / 2].addAdjacency({u, 2});
            u += l;
        }
    }
    return UndirectedGraph(nodes);
}

static uint64_t *generateRawGilbertGraph(uint64_t order, double p,
                                         std::mt19937_64 *gen) {
    uint64_t size = 0;
    uint64_t *edgeArray = new uint64_t[order];
    std::binomial_distribution<uint64_t> dist(order - 1, p);
    for (uint64_t i = 0; i < order; ++i) {
        uint64_t edges = dist(*gen);
        dist.reset();
        size += edges;
        edgeArray[i] = edges;
    }
    uint64_t *graph = new uint64_t[order + size + 2];

    graph[0] = order;
    graph[order + 1] = size;

    uint64_t lastPosition = order + 2;
    for (uint64_t i = 1; i <= order; ++i) {
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
    std::uniform_int_distribution<uint64_t> dist2(0, order - 1);
    for (uint64_t i = 0; i < order; ++i) {
        uint64_t numBitsSet = 0;
        // a[i] = number of edges from i
        uint64_t bitsToSet = edgeArray[i];
        if (initialBit == 1) {
            bitsToSet = order - bitsToSet;
        }
        while (edgeArray[i] > numBitsSet) {
            uint64_t rnd = dist2(rndDev);
            if (bitVector[rnd] == initialBit && rnd != i) {
                bitVector[rnd] = !initialBit;
                numBitsSet += 1;
            }
        }
        uint64_t pos = graph[i + 1];
        for (uint64_t j = 0; j < order; ++j) {
            if (bitVector[j] == !initialBit) {
                graph[pos++] = j + 1;
                bitVector[j] = initialBit;
            }
        }
    }
    delete[] edgeArray;
    return graph;
}

uint64_t *GraphCreator::fastGraphGeneration(uint64_t n, uint64_t mPern) {
    uint64_t M = n * mPern;
    uint64_t N = n + M + 2;

    uint64_t *A = new uint64_t[N];
    A[0] = n;
    A[n + 1] = M;
    std::random_device rd;
    std::mt19937_64 mt(rd());
    mt.seed(std::mt19937_64::default_seed);

    A[1] = n + 2;

    for (uint64_t v = 1; v <= n; ++v) {
        // First: Create Pointer!
        if (v != n) {
            A[v + 1] = A[v] + mPern;
        }
        std::set<uint64_t> generated;
        for (uint64_t p = A[v]; p < N && (v == n || p < A[v + 1]); p++) {
            std::uniform_int_distribution<uint64_t> dist(1, n);
            uint64_t rn = dist(mt);
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

CompactGraph GraphCreator::generateGilbertGraph(uint64_t order, double p,
                                                std::mt19937_64 *gen) {
    return CompactGraph(generateRawGilbertGraph(order, p, gen));
}
