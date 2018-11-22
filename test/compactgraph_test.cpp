#include <gtest/gtest.h>
#include <stdlib.h>
#include <sealib/graph/graphcreator.h>
#include <sealib/graph/compactgraph.h>

using Sealib::GraphCreator;
using Sealib::CompactGraph;

TEST(CompactGraphTest, compactgraph_integrity) {
    uint *A = new uint[16] {5,  7, 9, 11, 13, 15,  9,  2, 4,  3, 4,  1, 2,  2, 3,  3};
    CompactGraph *g = new CompactGraph(A);

    ASSERT_EQ(g->getOrder(), 5);

    ASSERT_EQ(g->getNodeDegree(0), 2);
    ASSERT_EQ(g->getNodeDegree(1), 2);
    ASSERT_EQ(g->getNodeDegree(2), 2);
    ASSERT_EQ(g->getNodeDegree(3), 2);
    ASSERT_EQ(g->getNodeDegree(4), 1);

    // vertex enumeration start with 0 in compact graph representation
    ASSERT_EQ(g->head(0, 0), 2);
    ASSERT_EQ(g->head(0, 1), 4);
    ASSERT_EQ(g->head(4, 0), 3);

    delete g;
}

TEST(CompactGraphTest, fastGraphGeneration) {
    uint *A = GraphCreator::fastGraphGeneration(5, 2);
    CompactGraph *g = new CompactGraph(A);

    ASSERT_EQ(g->getOrder(), 5);

    ASSERT_LE(g->head(0, 0), 5);
    ASSERT_LE(g->head(0, 1), 5);
    ASSERT_LE(g->head(1, 0), 5);
    ASSERT_LE(g->head(1, 1), 5);
    ASSERT_LE(g->head(2, 0), 5);
    ASSERT_LE(g->head(2, 1), 5);
    ASSERT_LE(g->head(3, 0), 5);
    ASSERT_LE(g->head(3, 1), 5);
    ASSERT_LE(g->head(4, 0), 5);
    ASSERT_LE(g->head(4, 1), 5);

    ASSERT_EQ(g->getNodeDegree(0), 2);
    ASSERT_EQ(g->getNodeDegree(1), 2);
    ASSERT_EQ(g->getNodeDegree(2), 2);
    ASSERT_EQ(g->getNodeDegree(3), 2);
    ASSERT_EQ(g->getNodeDegree(4), 2);

    delete g;
}

TEST(CompactGraphTest, gilbertGraphGeneration) {
    std::mt19937_64 gen;
    CompactGraph g = GraphCreator::generateGilbertGraph(32, 0.3, &gen);
    ASSERT_EQ(g.getOrder(), 32);
}
