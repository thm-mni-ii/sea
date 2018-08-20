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
