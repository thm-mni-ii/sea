#include "sealib/iterator/bfs.h"
#include <gtest/gtest.h>
#include <cstdio>
#include "../src/bfs/simplebfs.h"
#include "sealib/_types.h"
#include "sealib/graph/directedgraph.h"
#include "sealib/graph/graphcreator.h"

using namespace Sealib;  // NOLINT

static uint64_t GRAPHCOUNT = 4, ORDER = 5000, DEGREE = 20;

TEST(BFSTest, comparison) {
    for (uint64_t a = 0; a < GRAPHCOUNT; a++) {
        DirectedGraph g = GraphCreator::kOutdegree(ORDER, DEGREE);
        uint64_t c1 = 0, c2 = 0, c3 = 0, c4 = 0;
        BFS b1(
            g, [&c1](uint64_t) { c1++; }, [&c2](uint64_t, uint64_t) { c2++; });
        SimpleBFS b2(
            g, [&c3](uint64_t) { c3++; }, [&c4](uint64_t, uint64_t) { c4++; });
        b1.init();
        b2.init();
        do {
            while (b1.more()) {
                EXPECT_TRUE(b2.more());
                b1.next();
                b2.next();
            }
            if (b1.nextComponent()) {
                EXPECT_TRUE(b2.nextComponent());
                continue;
            }
        } while (false);
        EXPECT_EQ(c1, ORDER);
        EXPECT_EQ(c2, ORDER * DEGREE);
        EXPECT_EQ(c3, ORDER);
        EXPECT_EQ(c4, ORDER * DEGREE);
    }
}

TEST(BFSTest, nextComponent) {
    uint64_t c1 = 0, c2 = 0;
    DirectedGraph g = GraphCreator::kOutdegree(ORDER, 0);
    BFS bfs(
        g, [&c1](uint64_t) { c1++; }, [&c2](uint64_t, uint64_t) { c2++; });
    uint64_t cc = 0;
    bfs.forEach([&cc](std::pair<uint64_t, uint64_t>) { cc++; });
    EXPECT_EQ(cc, ORDER);
    EXPECT_EQ(c1, ORDER);
    EXPECT_EQ(c2, 0);
}

TEST(SimpleBFSTest, nextComponent) {
    uint64_t c1 = 0, c2 = 0;
    DirectedGraph g = GraphCreator::kOutdegree(ORDER, 0);
    SimpleBFS bfs(
        g, [&c1](uint64_t) { c1++; }, [&c2](uint64_t, uint64_t) { c2++; });
    uint64_t cc = 0;
    bfs.forEach([&cc](std::pair<uint64_t, uint64_t>) { cc++; });
    EXPECT_EQ(cc, ORDER);
    EXPECT_EQ(c1, ORDER);
    EXPECT_EQ(c2, 0);
}
