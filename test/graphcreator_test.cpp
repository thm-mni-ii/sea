#include "sealib/graph/graphcreator.h"
#include <gtest/gtest.h>
#include <cstdlib>
#include <random>

using namespace Sealib;  // NOLINT

static uint n = 200, deg = 15;
static std::random_device rnd;
static std::uniform_int_distribution<uint> nR(0, n - 1);
static std::uniform_int_distribution<uint> degR(0, deg - 1);

TEST(GraphCreatorTest, adjMatrix) {
    std::uniform_int_distribution<uint> edgeR(0, 2);
    uint **m = static_cast<uint **>(std::malloc(n * sizeof(uint *)));
    for (uint u = 0; u < n; u++) {
        m[u] = static_cast<uint *>(std::malloc(n * sizeof(uint)));
    }
    for (uint u = 0; u < n; u++) {
        for (uint v = 0; v < n; v++) {
            uint edges = edgeR(rnd);
            m[u][v] = edges;
            m[v][u] = edges;
        }
    }
    UndirectedGraph g = GraphCreator::createFromAdjacencyMatrix(m, n);
    SUCCEED();
}

TEST(GraphCreatorTest, kRegular) {
    UndirectedGraph g = GraphCreator::kRegular(n, deg);
    for (uint u = 0; u < n; u++) {
        EXPECT_EQ(g.deg(u), deg);
    }
}

TEST(GraphCreatorTest, kOutdegree) {
    DirectedGraph g = GraphCreator::kOutdegree(n, deg);
    for (uint u = 0; u < n; u++) {
        EXPECT_EQ(g.deg(u), deg);
    }
}

TEST(GraphCreatorTest, sparseUndirected) {
    UndirectedGraph g = GraphCreator::sparseUndirected(n);
    for (uint u = 0; u < n; u++) {
        EXPECT_LT(g.deg(u), n);
    }
}

TEST(GraphCreatorTest, sparseDirected) {
    DirectedGraph g = GraphCreator::sparseDirected(n);
    for (uint u = 0; u < n; u++) {
        EXPECT_LE(g.deg(u), log2(n));
    }
}

TEST(GraphCreatorTest, windmill) {
    uint m = 20;
    UndirectedGraph g = GraphCreator::windmill(n, m);
    for (uint u = 0; u < n * m - m; u++) {
        EXPECT_EQ(g.deg(u), n - 1);
    }
    EXPECT_EQ(g.deg(n * m - m), n * m - m);
}
