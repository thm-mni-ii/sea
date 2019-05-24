#include "sealib/iterator/bcciterator.h"
#include <gtest/gtest.h>
#include "sealib/graph/graphcreator.h"

namespace Sealib {

static std::set<uint64_t> nodes;
static std::set<std::set<uint64_t>> edges;

static void compareWindmill() {
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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated"

TEST(BCCIteratorTest, windmillGraph) {
    UndirectedGraph g = GraphCreator::windmill(5, 4);
    BCCIterator b(g);
    b.init(16);
    nodes.clear();
    edges.clear();
    b.forEach([&](std::pair<uint64_t, uint64_t> n) {
        if (n.second == INVALID) {
            nodes.insert(n.first);
        } else {
            edges.insert({n.first, n.second});
        }
    });
    compareWindmill();
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

#pragma clang diagnostic pop

TEST(BCCOutputTest, windmillGraph) {
    UndirectedGraph g = GraphCreator::windmill(5, 4);
    BCCOutput b(g);
    nodes.clear();
    edges.clear();
    b.traverse(16, [](uint64_t u) { nodes.insert(u); },
               [](uint64_t u, uint64_t v) {
                   edges.insert({u, v});
               });
    compareWindmill();
}

TEST(BCCOutput, cycle) {
    for (uint64_t k = 0; k < 20; k++) {
        UndirectedGraph g = GraphCreator::cycle(1000, k);
        uint64_t n = 0, m = 0;
        BCCOutput(g).traverse(1, [&n](uint64_t) { n++; },
                              [&m](uint64_t, uint64_t) { m++; });
        EXPECT_EQ(n, 1000);
        EXPECT_EQ(m, 1000 + k);
    }
}

}  // namespace Sealib
