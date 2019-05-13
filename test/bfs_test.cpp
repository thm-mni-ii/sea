#include "sealib/iterator/bfs.h"
#include <gtest/gtest.h>
#include <stdio.h>
#include "../src/bfs/simplebfs.h"
#include "sealib/_types.h"
#include "sealib/graph/graph.h"
#include "sealib/graph/graphcreator.h"

using namespace Sealib;  // NOLINT

static uint64_t GRAPHCOUNT = 4, ORDER = 5000, DEGREE = 20;
static std::vector<DirectedGraph> makeGraphs() {
    std::vector<DirectedGraph> g;
    for (uint64_t c = 0; c < GRAPHCOUNT; c++) {
        g.emplace_back(GraphCreator::kOutdegree(ORDER, DEGREE));
    }
    return g;
}

class BFSTest : public ::testing::TestWithParam<DirectedGraph> {};

INSTANTIATE_TEST_CASE_P(ParamTests, BFSTest, ::testing::ValuesIn(makeGraphs()),
                        /**/);

TEST_P(BFSTest, userproc) {
    uint64_t c1 = 0, c2 = 0;
    DirectedGraph const& g = GetParam();
    BFS bfs(
        g, [&](uint64_t) { c1++; }, [&](uint64_t, uint64_t) { c2++; });
    bfs.forEach([](std::pair<uint64_t, uint64_t>) {});
    EXPECT_EQ(c1, ORDER);
    EXPECT_EQ(c2, ORDER * DEGREE);
}

TEST_P(BFSTest, stdUserproc) {
    uint64_t c1 = 0, c2 = 0;
    DirectedGraph const& g = GetParam();
    SimpleBFS bfs(
        g, [&](uint64_t) { c1++; }, [&](uint64_t, uint64_t) { c2++; });
    bfs.forEach([](std::pair<uint64_t, uint64_t>) {});
    EXPECT_EQ(c1, ORDER);
    EXPECT_EQ(c2, ORDER * DEGREE);
}

TEST(BFSTest, nextComponent) {
    uint64_t c1 = 0, c2 = 0;
    DirectedGraph g = GraphCreator::kOutdegree(ORDER, 0);
    BFS bfs(
        g, [&](uint64_t) { c1++; }, [&](uint64_t, uint64_t) { c2++; });
    uint64_t cc = 0;
    bfs.forEach([&](std::pair<uint64_t, uint64_t>) { cc++; });
    EXPECT_EQ(cc, ORDER);
    EXPECT_EQ(c1, ORDER);
    EXPECT_EQ(c2, 0);
}

TEST(SimpleBFSTest, nextComponent) {
    uint64_t c1 = 0, c2 = 0;
    DirectedGraph g = GraphCreator::kOutdegree(ORDER, 0);
    SimpleBFS bfs(
        g, [&](uint64_t) { c1++; }, [&](uint64_t, uint64_t) { c2++; });
    uint64_t cc = 0;
    bfs.forEach([&](std::pair<uint64_t, uint64_t>) { cc++; });
    EXPECT_EQ(cc, ORDER);
    EXPECT_EQ(c1, ORDER);
    EXPECT_EQ(c2, 0);
}
