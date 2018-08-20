#include <gtest/gtest.h>
#include <stdlib.h>
#include <sealib/graphcreator.h>
#include <sealib/compactgraph.h>

using Sealib::GraphCreator;
using Sealib::Basicgraph;
using Sealib::Compactgraph;
using Sealib::Graph;

TEST(GraphTest, compactgraph_integrity) {
    unsigned int *A = new unsigned int[14] {5,  7, 9, 11, 13, 14,  7,  2, 4,  3, 4,  1, 2,  3};
    Compactgraph *g = new Compactgraph(A);

    ASSERT_EQ(g->getOrder(), 5);

    ASSERT_EQ(g->getNodeDegree(1), 2);
    ASSERT_EQ(g->getNodeDegree(2), 2);
    ASSERT_EQ(g->getNodeDegree(3), 2);
    ASSERT_EQ(g->getNodeDegree(4), 2);
    ASSERT_EQ(g->getNodeDegree(5), 1);

    // vertex enumeration start with 0 in compact graph representation
    ASSERT_EQ(g->head(0, 0), 2);
    ASSERT_EQ(g->head(0, 1), 4);
    ASSERT_EQ(g->head(5, 0), 3);

    delete(g);
}
