#include "sealib/graph/graphcreator.h"
#include <gtest/gtest.h>
#include <cstdio>
#include <cstdlib>
#include <random>

using namespace Sealib;  // NOLINT

static uint64_t n = 200, deg = 15;
static std::random_device rnd;
static std::uniform_int_distribution<uint64_t> nR(0, n - 1);
static std::uniform_int_distribution<uint64_t> degR(0, deg - 1);

TEST(GraphCreatorTest, adjMatrix) {
    std::uniform_int_distribution<uint64_t> edgeR(0, 2);
    uint64_t **m =
        static_cast<uint64_t **>(std::malloc(n * sizeof(uint64_t *)));
    for (uint64_t u = 0; u < n; u++) {
        m[u] = static_cast<uint64_t *>(std::malloc(n * sizeof(uint64_t)));
    }
    for (uint64_t u = 0; u < n; u++) {
        for (uint64_t v = 0; v < n; v++) {
            uint64_t edges = edgeR(rnd);
            m[u][v] = edges;
            m[v][u] = edges;
        }
    }
    UndirectedGraph g = GraphCreator::createFromAdjacencyMatrix(m, n);
    SUCCEED();
}

TEST(GraphCreatorTest, kRegular) {
    UndirectedGraph g = GraphCreator::kRegular(n, deg);
    for (uint64_t u = 0; u < n; u++) {
        EXPECT_EQ(g.deg(u), deg);
    }
}

TEST(GraphCreatorTest, kOutdegree) {
    DirectedGraph g = GraphCreator::kOutdegree(n, deg);
    for (uint64_t u = 0; u < n; u++) {
        EXPECT_EQ(g.deg(u), deg);
    }
}

TEST(GraphCreatorTest, sparseUndirected) {
    UndirectedGraph g = GraphCreator::sparseUndirected(n);
    for (uint64_t u = 0; u < n; u++) {
        EXPECT_LT(g.deg(u), n);
    }
}

TEST(GraphCreatorTest, sparseDirected) {
    DirectedGraph g = GraphCreator::sparseDirected(n);
    for (uint64_t u = 0; u < n; u++) {
        EXPECT_LE(g.deg(u), log2(n));
    }
}

TEST(GraphCreatorTest, windmill) {
    uint64_t m = 20;
    UndirectedGraph g = GraphCreator::windmill(n, m);
    for (uint64_t u = 0; u < n * m - m; u++) {
        EXPECT_EQ(g.deg(u), n - 1);
    }
    EXPECT_EQ(g.deg(n * m - m), n * m - m);
}

TEST(GraphCreatorTest, transpose) {
    DirectedGraph g = GraphCreator::kOutdegree(n, deg);
    DirectedGraph t = GraphCreator::transpose(g);
    for (uint64_t u = 0; u < t.getOrder(); u++) {
        for (uint64_t k = 0; k < t.deg(u); k++) {
            uint64_t v = t.head(u, k);
            bool hasAdj = false;
            for (uint64_t j = 0; j < g.deg(u) && !hasAdj; j++) {
                if (g.head(v, j) == u) hasAdj = true;
            }
            EXPECT_TRUE(hasAdj);
        }
    }
}
