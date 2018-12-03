#include "sealib/iterator/bcciterator.h"
#include <gtest/gtest.h>
#include "sealib/graph/graphcreator.h"

namespace Sealib {

TEST(BCCIteratorTest, windmillGraph) {
    UndirectedGraph g = GraphCreator::createWindmill(5, 4);
    BCCIterator b(&g);
    b.init();
    b.start(1, 16);
    std::set<uint> nodes;
    std::set<std::set<uint>> edges;
    while (b.more()) {
        std::pair<uint, uint> n = b.next();
        if (n.second == INVALID) {
            nodes.insert(n.first);
        } else {
            edges.insert({n.first, n.second});
        }
    }

    EXPECT_EQ(nodes.size(), 5);
    EXPECT_EQ(edges.size(), 7);

    EXPECT_NE(nodes.find(16), nodes.end());
    EXPECT_NE(nodes.find(3), nodes.end());
    EXPECT_NE(nodes.find(2), nodes.end());
    EXPECT_NE(nodes.find(1), nodes.end());
    EXPECT_NE(nodes.find(0), nodes.end());

    // back edges should only be included when reaching a vertex via a
    // full-marked edge (see paper, p. 8)
    EXPECT_NE(edges.find({16, 3}), edges.end());
    EXPECT_EQ(edges.find({16, 2}), edges.end());  // not-included back edge
    EXPECT_EQ(edges.find({16, 1}), edges.end());  // not-included back edge
    EXPECT_EQ(edges.find({16, 0}), edges.end());  // not-included back edge
    EXPECT_NE(edges.find({3, 2}), edges.end());
    EXPECT_NE(edges.find({3, 1}), edges.end());  // back edge
    EXPECT_NE(edges.find({3, 0}), edges.end());  // back edge
    EXPECT_NE(edges.find({2, 1}), edges.end());
    EXPECT_NE(edges.find({2, 0}), edges.end());  // back edge
    EXPECT_NE(edges.find({1, 0}), edges.end());  // half marked
}

TEST(BCCIteratorTest, lineGraph) {
    uint size = 10;
    UndirectedGraph g(size);
    for (uint a = 0; a < size - 1; a++) {
        uint i1 = g.getNodeDegree(a), i2 = g.getNodeDegree(a + 1);
        g.getNode(a).addAdjacency(a + 1);
        g.getNode(a).setCrossIndex(i1, i2);
        g.getNode(a + 1).addAdjacency(a);
        g.getNode(a + 1).setCrossIndex(i2, i1);
    }

    BCCIterator b(&g);
    b.init();
    b.start(4, 5);

    std::set<uint> nodes;
    std::set<std::set<uint>> edges;
    while (b.more()) {
        std::pair<uint, uint> n = b.next();
        if (n.second == INVALID) {
            nodes.insert(n.first);
        } else {
            edges.insert({n.first, n.second});
        }
    }
    EXPECT_EQ(nodes.size(), 2);
    EXPECT_EQ(edges.size(), 1);
    EXPECT_NE(nodes.find(4), nodes.end());
    EXPECT_NE(nodes.find(5), nodes.end());
    EXPECT_NE(edges.find({4, 5}), edges.end());
}

}  // namespace Sealib
