#include "sealib/bcciterator.h"
#include <gtest/gtest.h>
#include "sealib/graphcreator.h"

using namespace Sealib;  // NOLINT

TEST(BCCIteratorTest, windmillGraph) {
    BasicGraph *g = GraphCreator::createWindmill(5, 4);
    BCCIterator b(g);
    b.init();
    b.start(1, 16);
    std::set<uint> nodes;
    std::set<std::set<uint>> edges;
    while (b.more()) {
        Pair n = b.next();
        std::cout << n.head() << "," << n.tail() << " ";
        if (n.tail() == INVALID) {
            nodes.insert(n.head());
        } else {
            edges.insert({n.head(), n.tail()});
        }
    }
    EXPECT_NE(nodes.find(16), nodes.end());
    EXPECT_NE(nodes.find(3), nodes.end());
    EXPECT_NE(nodes.find(2), nodes.end());
    EXPECT_NE(nodes.find(1), nodes.end());
    EXPECT_EQ(nodes.find(0), nodes.end());

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
