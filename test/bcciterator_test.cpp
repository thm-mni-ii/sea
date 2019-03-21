#include "sealib/iterator/bcciterator.h"
#include <gtest/gtest.h>
#include "sealib/graph/graphcreator.h"

namespace Sealib {

TEST(BCCIteratorTest, windmillGraph) {
    UndirectedGraph g = GraphCreator::windmill(5, 4);
    BCCIterator b(g);
    b.init(16);
    std::set<uint64_t> nodes;
    std::set<std::set<uint64_t>> edges;
    b.forEach([&](std::pair<uint64_t, uint64_t> n) {
        if (n.second == INVALID) {
            nodes.insert(n.first);
        } else {
            edges.insert({n.first, n.second});
        }
    });

    EXPECT_EQ(nodes.size(), 5);
    EXPECT_EQ(edges.size(), 10);

    EXPECT_NE(nodes.find(16), nodes.end());
    EXPECT_NE(nodes.find(3), nodes.end());
    EXPECT_NE(nodes.find(2), nodes.end());
    EXPECT_NE(nodes.find(1), nodes.end());
    EXPECT_NE(nodes.find(0), nodes.end());

    // Back edges to the BCC root are also output
    EXPECT_NE(edges.find({16, 3}), edges.end());
    EXPECT_NE(edges.find({16, 2}), edges.end());  // back edge to root
    EXPECT_NE(edges.find({16, 1}), edges.end());  // back edge to root
    EXPECT_NE(edges.find({16, 0}), edges.end());  // back edge to root
    EXPECT_NE(edges.find({3, 2}), edges.end());
    EXPECT_NE(edges.find({3, 1}), edges.end());  // back edge
    EXPECT_NE(edges.find({3, 0}), edges.end());  // back edge
    EXPECT_NE(edges.find({2, 1}), edges.end());
    EXPECT_NE(edges.find({2, 0}), edges.end());  // back edge
    EXPECT_NE(edges.find({1, 0}), edges.end());  // half marked
}

TEST(BCCIteratorTest, lineGraph) {
    uint64_t size = 10;
    UndirectedGraph g(size);
    for (uint64_t a = 0; a < size - 1; a++) {
        uint64_t i1 = g.deg(a), i2 = g.deg(a + 1);
        g.getNode(a).addAdjacency({a + 1, i2});
        g.getNode(a + 1).addAdjacency({a, i1});
    }

    BCCIterator b(g);
    b.init(5);

    ASSERT_TRUE(b.more());
    EXPECT_EQ(b.next(), (std::pair<uint64_t, uint64_t>{5, INVALID}));
    ASSERT_TRUE(b.more());
    EXPECT_EQ(b.next(), (std::pair<uint64_t, uint64_t>{5, 4}));
    ASSERT_TRUE(b.more());
    EXPECT_EQ(b.next(), (std::pair<uint64_t, uint64_t>{4, INVALID}));
    ASSERT_FALSE(b.more());
}

TEST(BCCIteratorTest, stability) {
    UndirectedGraph g = GraphCreator::kRegular(2000, 3);
    BCCIterator b(g);
    b.init(g.head(10, 2));  // select an arbitrary edge
    while (b.more()) b.next();
    SUCCEED();
}

}  // namespace Sealib
