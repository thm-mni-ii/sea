#include <gtest/gtest.h>
#include <stdlib.h>
#include <sealib/graphcreator.h>
#include <sealib/compactgraph.h>
#include <sealib/graphrepresentations.h>

using Sealib::GraphCreator;
using Sealib::Basicgraph;
using Sealib::Graph;
using Sealib::Compactgraph;
using Sealib::Graphrepresentations;

TEST(GraphTest, graph_generation) {
    uint *A = Graphrepresentations::fastGraphGeneration(5, 2);
    Compactgraph *g = new Compactgraph(A);

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

TEST(GraphTest, graph_beginpointer) {
    uint *A = new uint[16] {5,  7, 9, 11, 13, 15,  9,  2, 4,  3, 4,  1, 2,  2, 3,  3};

    Graphrepresentations::standardToBeginpointer(A);

    ASSERT_EQ(A[0], 5);

    ASSERT_EQ(A[7], 9);
    ASSERT_EQ(A[9], 11);
    ASSERT_EQ(A[11], 7);
    ASSERT_EQ(A[13], 9);
    ASSERT_EQ(A[15], 11);
}
