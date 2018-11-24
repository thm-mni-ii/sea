#include <gtest/gtest.h>
#include <iostream>
#include "sealib/iterator/cutvertexiterator.h"
#include "sealib/graph/graphcreator.h"

namespace Sealib {

TEST(CutVertexIteratorTest, windmillGraph) {
    UndirectedGraph g = GraphCreator::createWindmill(3, 4);
    CutVertexIterator c(&g);
    c.init();

    // there should be exactly one cut vertex (the windmill's center, which has
    // the last index in G)
    EXPECT_TRUE(c.more());
    EXPECT_EQ(c.next(), g.getOrder() - 1);
    EXPECT_FALSE(c.more());

    for (uint a = 0; a < g.getOrder() - 1; a++) {
        EXPECT_FALSE(c.isCutVertex(a));
    }
    EXPECT_TRUE(c.isCutVertex(g.getOrder() - 1));
}

TEST(CutVertexIteratorTest, lineGraph) {
    uint n = 20;
    UndirectedGraph g(n);
    for (uint a = 0; a < n - 1; a++) {
        uint i1 = g.getNodeDegree(a), i2 = g.getNodeDegree(a + 1);
        g.getNode(a).addAdjacency(a + 1);
        g.getNode(a).setCrossIndex(i1, i2);
        g.getNode(a + 1).addAdjacency(a);
        g.getNode(a + 1).setCrossIndex(i2, i1);
    }

    CutVertexIterator c(&g);
    c.init();

    EXPECT_FALSE(c.isCutVertex(0));
    for (uint a = 1; a < n - 1; a++) {
        EXPECT_TRUE(c.isCutVertex(a));
    }
    EXPECT_FALSE(c.isCutVertex(n - 1));
}

}  // namespace Sealib
