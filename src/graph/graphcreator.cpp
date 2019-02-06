#include <sealib/graph/graphcreator.h>
#include <stdlib.h>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <random>
#include <set>
#include <unordered_set>

using Sealib::DirectedGraph;
using Sealib::UndirectedGraph;
using Sealib::CompactGraph;
using Sealib::GraphCreator;

UndirectedGraph *Sealib::GraphCreator::createPointerFromAdjacencyMatrix(
    uint **adjMatrix, uint order) {
    std::vector<ExtendedNode> nodes;

    for (uint i = 0; i < order; i++) {
        std::vector<std::pair<uint, uint>> adj;

        for (uint j = 0; j < order; j++) {
            for (uint k = 0; k < adjMatrix[i][j]; k++)
                adj.push_back({j, INVALID});
        }

        nodes.push_back(ExtendedNode(adj));
    }
    for (uint i = 0; i < order; i++) {
        const uint deg1 = nodes[i].getDegree();
        std::vector<std::pair<uint, uint>> &n1 = nodes[i].getAdj();

        for (uint k1 = 0; k1 < deg1; k1++) {
            if (n1[k1].second == INVALID) {
                uint v = n1[k1].first;
                std::vector<std::pair<uint, uint>> &n2 = nodes[v].getAdj();
                const uint deg2 = nodes[v].getDegree();

                for (uint k2 = 0; k2 < deg2; k2++) {
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

UndirectedGraph GraphCreator::createFromAdjacencyMatrix(uint **adjMatrix,
                                                        uint order) {
    return *createPointerFromAdjacencyMatrix(adjMatrix, order);
}

std::shared_ptr<UndirectedGraph>
GraphCreator::createSharedPointerFromAdjacencyMatrix(uint **adjMatrix,
                                                     uint order) {
    return std::shared_ptr<UndirectedGraph>(
        createPointerFromAdjacencyMatrix(adjMatrix, order));
}

std::unique_ptr<Sealib::UndirectedGraph> Sealib::GraphCreator::randomBipartite(
    uint order1, uint order2, double p, uint seed) {
    std::unique_ptr<Sealib::UndirectedGraph> graph(
        new Sealib::UndirectedGraph(order1 + order2));

    std::mt19937_64 _rng(seed);
    std::uniform_real_distribution<double> unif(0.0, 1.0);

    for (uint n1 = 0; n1 < order1; n1++) {
        for (uint n2 = order1; n2 < order2; n2++) {
            if (unif(_rng) < p) {
                ExtendedNode &node1 = graph->getNode(n1);
                ExtendedNode &node2 = graph->getNode(n2);

                uint n1idx = node1.getDegree();
                uint n2idx = node2.getDegree();

                node1.addAdjacency({n2, n2idx});

                node2.addAdjacency({n1, n1idx});
            }
        }
    }

    return graph;
}

static std::random_device rng;

Sealib::DirectedGraph GraphCreator::imbalanced(uint order) {
    std::vector<SimpleNode> n(order);
    std::uniform_int_distribution<uint> rnd(0, order - 1);
    std::uniform_int_distribution<uint> dist1(order * order, 2 * order * order);
    std::uniform_int_distribution<uint> dist2(
        0, static_cast<uint>(ceil(log2(order))));
    std::set<uint> big;
    for (uint a = 0; a < ceil(order / (2 * log2(order))); a++)
        big.insert(rnd(rng));
    for (uint a = 0; a < order; a++) {
        uint deg;
        if (big.find(a) == big.end()) {
            deg = dist2(rng);
        } else {
            deg = dist1(rng);
        }
        std::vector<uint> ad(deg);
        for (uint b = 0; b < deg; b++) {
            ad[b] = rnd(rng);
        }
        n[a] = SimpleNode(ad);
    }
    return DirectedGraph(n);
}

Sealib::DirectedGraph Sealib::GraphCreator::kOutdegree(uint order,
                                                       uint degreePerNode) {
    std::uniform_int_distribution<uint> rnd(0, order - 1);
    std::vector<SimpleNode> n(order);
    for (uint a = 0; a < order; a++) {
        std::vector<uint> ad(degreePerNode);
        for (uint b = 0; b < degreePerNode; b++) {
            ad[b] = rnd(rng);
        }
        n[a] = SimpleNode(ad);
    }
    return DirectedGraph(n);
}

Sealib::DirectedGraph Sealib::GraphCreator::sparseDirected(uint order) {
    std::vector<SimpleNode> n(order);
    std::uniform_int_distribution<uint> nR(0, order - 1);
    std::uniform_int_distribution<uint> degR(0, static_cast<uint>(log2(order)));
    for (uint a = 0; a < order; a++) {
        uint deg = degR(rng);
        std::vector<uint> ad(deg);
        for (uint b = 0; b < deg; b++) {
            ad[b] = nR(rng);
        }
        n[a] = SimpleNode(ad);
    }
    return DirectedGraph(n);
}

UndirectedGraph GraphCreator::sparseUndirected(uint order) {
    UndirectedGraph g(order);
    std::uniform_int_distribution<uint> nR(0, order - 1);
    std::uniform_int_distribution<uint> degR(0, static_cast<uint>(log2(order)));
    for (uint a = 0; a < order; a++) {
        for (uint c = 0; c < degR(rng); c++) {
            uint b = nR(rng);
            ExtendedNode &n1 = g.getNode(a), &n2 = g.getNode(b);
            uint i1 = g.deg(a), i2 = g.deg(b);
            n1.addAdjacency({b, i2});
            n2.addAdjacency({a, i1});
        }
    }
    return g;
}

UndirectedGraph GraphCreator::kRegular(uint order, uint degreePerNode) {
    UndirectedGraph g(order);
    std::uniform_int_distribution<uint> dist(0, order - 1);
    std::unordered_set<uint> todo;
    for (uint a = 0; a < order; a++) {
        todo.insert(a);
    }
    while (!todo.empty()) {
        uint a = *todo.begin();
        todo.erase(a);
        uint b;
        if (!todo.empty()) {
            auto todoI = todo.begin();
            std::advance(todoI, dist(rng) % todo.size());
            b = *todoI;
            todo.erase(b);
        } else {
            b = a;
        }
        ExtendedNode &n1 = g.getNode(a), &n2 = g.getNode(b);
        uint i1 = g.deg(a), i2 = g.deg(b);
        n1.addAdjacency({b, i2});
        n2.addAdjacency({a, i1});
        if (g.deg(a) < degreePerNode) {
            todo.insert(a);
        }
        if (g.deg(b) < degreePerNode) {
            todo.insert(b);
        }
    }
    return g;
}

UndirectedGraph GraphCreator::windmill(uint order, uint count) {
    order--;
    uint n = order * count + 1;
    UndirectedGraph g(n);
    for (uint a = 0; a < count; a++) {
        // a = no. complete graphs
        for (uint b = a * order; b < (a + 1) * order - 1; b++) {
            // b = no. source nodes
            for (uint c = b + 1; c < (a + 1) * order; c++) {
                // c = no. dest. nodes
                uint i1 = g.deg(b), i2 = g.deg(c);
                g.getNode(b).addAdjacency({c, i2});
                g.getNode(c).addAdjacency({b, i1});
            }
        }
    }
    for (uint a = 0; a < n - 1; a++) {
        uint i1 = g.deg(n - 1), i2 = g.deg(a);
        g.getNode(n - 1).addAdjacency({a, i2});
        g.getNode(a).addAdjacency({n - 1, i1});
    }
    return g;
}

static uint *generateRawGilbertGraph(uint order, double p,
                                     std::mt19937_64 *gen) {
    uint size = 0;
    uint *edgeArray = new uint[order];
    std::binomial_distribution<uint> dist(order - 1, p);
    for (uint i = 0; i < order; ++i) {
        uint edges = dist(*gen);
        dist.reset();
        size += edges;
        edgeArray[i] = edges;
    }
    uint *graph = new uint[order + size + 2];

    graph[0] = order;
    graph[order + 1] = size;

    uint lastPosition = order + 2;
    for (uint i = 1; i <= order; ++i) {
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
    std::uniform_int_distribution<uint> dist2(0, order - 1);
    for (uint i = 0; i < order; ++i) {
        uint numBitsSet = 0;
        // a[i] = number of edges from i
        uint bitsToSet = edgeArray[i];
        if (initialBit == 1) {
            bitsToSet = order - bitsToSet;
        }
        while (edgeArray[i] > numBitsSet) {
            uint rnd = dist2(rndDev);
            if (bitVector[rnd] == initialBit && rnd != i) {
                bitVector[rnd] = !initialBit;
                numBitsSet += 1;
            }
        }
        uint pos = graph[i + 1];
        for (uint j = 0; j < order; ++j) {
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

    uint *A = new uint[N];
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

CompactGraph GraphCreator::generateGilbertGraph(uint order, double p,
                                                std::mt19937_64 *gen) {
    return CompactGraph(generateRawGilbertGraph(order, p, gen));
}
