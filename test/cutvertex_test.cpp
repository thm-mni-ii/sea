#include <gtest/gtest.h>
#include <iostream>
#include "../src/marker/simplecutvertexiterator.h"
#include "sealib/graph/graphcreator.h"
#include "sealib/iterator/cutvertexiterator.h"

namespace Sealib {

TEST(CutVertexIteratorTest, windmillGraph) {
    UndirectedGraph g = GraphCreator::windmill(3, 4);
    CutVertexIterator c(g);
    c.init();

    // there should be exactly one cut vertex (the windmill's center, which has
    // the last index in G)
    EXPECT_TRUE(c.more());
    EXPECT_EQ(c.next(), g.getOrder() - 1);
    EXPECT_FALSE(c.more());

    for (uint64_t a = 0; a < g.getOrder() - 1; a++) {
        EXPECT_FALSE(c.isCutVertex(a));
    }
    EXPECT_TRUE(c.isCutVertex(g.getOrder() - 1));
}

TEST(SimpleCutVertexIteratorTest, windmillGraph) {
    UndirectedGraph g = GraphCreator::windmill(3, 4);
    SimpleCutVertexIterator c(g);
    c.init();
    ASSERT_TRUE(c.more());
    EXPECT_EQ(c.next(), g.getOrder() - 1);
    EXPECT_FALSE(c.more());
}

TEST(CutVertexIteratorTest, lineGraph) {
    uint64_t n = 20;
    UndirectedGraph g(n);
    for (uint64_t a = 0; a < n - 1; a++) {
        uint64_t i1 = g.deg(a), i2 = g.deg(a + 1);
        g.getNode(a).addAdjacency({a + 1, i2});
        g.getNode(a + 1).addAdjacency({a, i1});
    }

    CutVertexIterator c(g);
    c.init();

    EXPECT_FALSE(c.isCutVertex(0));
    for (uint64_t a = 1; a < n - 1; a++) {
        EXPECT_TRUE(c.isCutVertex(a));
    }
    EXPECT_FALSE(c.isCutVertex(n - 1));
}

TEST(CutVertexIteratorTest, cycle) {
    SimpleCutVertexIterator(GraphCreator::kRegular(100000, 20)).init();
    return;
    for (uint64_t k = 0; k < 100; k++) {
        UndirectedGraph g = GraphCreator::cycle(500, k);
        CutVertexIterator c(g);
        c.init();
        EXPECT_FALSE(c.more());
    }
}

TEST(SimpleCutVertexIteratorTest, cycle) {
    for (uint64_t k = 0; k < 100; k++) {
        UndirectedGraph g = GraphCreator::cycle(500, k);
        SimpleCutVertexIterator c(g);
        c.init();
        EXPECT_FALSE(c.more());
    }
}

}  // namespace Sealib
