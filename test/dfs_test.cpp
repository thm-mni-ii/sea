#include "sealib/iterator/dfs.h"
#include <gtest/gtest.h>
#include <memory>
#include <random>
#include <stack>
#include <vector>
#include "sealib/graph/graphcreator.h"
#include "sealib/graph/undirectedgraph.h"

using std::stack;
using std::vector;

namespace Sealib {

static uint64_t c1 = 0, c2 = 0, c3 = 0, c4 = 0;

static void incr1(uint64_t) { c1++; }

static void incr4(uint64_t) { c4++; }

static void incr2(uint64_t, uint64_t) { c2++; }

static void incr3(uint64_t, uint64_t) { c3++; }

static std::random_device rnd;
static const uint64_t GRAPHCOUNT = 4;  // how many random graphs to generate?
static const uint64_t DEGREE = 15;     // how many outneighbours per node?
static const uint64_t ORDER = 2000;

static std::vector<DirectedGraph> makeDGraphs() {
    std::vector<DirectedGraph> g;
    for (uint64_t c = 0; c < GRAPHCOUNT; c++) {
        g.push_back(Sealib::GraphCreator::kOutdegree(ORDER, DEGREE));
    }
    return g;
}

static std::vector<UndirectedGraph> makeUGraphs() {
    std::vector<UndirectedGraph> g;
    for (uint64_t c = 0; c < 2 * GRAPHCOUNT; c++) {
        g.push_back(Sealib::GraphCreator::kRegular(5 * ORDER, DEGREE));
    }
    return g;
}

class DFSTest : public ::testing::TestWithParam<DirectedGraph> {
 protected:
    virtual void SetUp() { c1 = c2 = c3 = c4 = 0; }
};

class DFSTest2 : public ::testing::TestWithParam<UndirectedGraph> {
 protected:
    virtual void SetUp() { c1 = c2 = c3 = c4 = 0; }
};

INSTANTIATE_TEST_CASE_P(ParamTests, DFSTest, ::testing::ValuesIn(makeDGraphs()),
                        /**/);

INSTANTIATE_TEST_CASE_P(ParamTests, DFSTest2,
                        ::testing::ValuesIn(makeUGraphs()),
                        /**/);

TEST_P(DFSTest, stdUserproc) {
    DirectedGraph g = GetParam();
    DFS::standardDFS(g, incr1, incr2, incr3, incr4);
    EXPECT_EQ(c1, ORDER);
    EXPECT_EQ(c2, DEGREE * ORDER);
    EXPECT_EQ(c3, DEGREE * ORDER);
    EXPECT_EQ(c4, ORDER);
}

TEST_P(DFSTest, nBitUserproc) {
    DirectedGraph g = GetParam();
    DFS::nBitDFS(g, incr1, incr2, incr3, incr4);
    EXPECT_EQ(c1, ORDER);
    EXPECT_EQ(c2, DEGREE * ORDER);
    EXPECT_EQ(c3, DEGREE * ORDER);
    EXPECT_EQ(c4, ORDER);
}

TEST_P(DFSTest, nloglognBitUserproc) {
    DirectedGraph g = GetParam();
    DFS::nloglognBitDFS(g, incr1, incr2, incr3, incr4);
    EXPECT_EQ(c1, ORDER);
    EXPECT_EQ(c2, DEGREE * ORDER);
    EXPECT_EQ(c3, DEGREE * ORDER);
    EXPECT_EQ(c4, ORDER);
}

TEST_P(DFSTest2, nplusmBitUserproc) {
    UndirectedGraph g = GetParam();
    DFS::nplusmBitDFS(g, incr1, incr2, incr3, incr4);
    EXPECT_EQ(c1, 5 * ORDER);
    EXPECT_EQ(c2, 5 * ORDER * DEGREE);
    EXPECT_EQ(c3, 5 * ORDER * DEGREE);
    EXPECT_EQ(c4, 5 * ORDER);
}

TEST(DFSTest, nloglognImbalanced) {
    c1 = c2 = c3 = c4 = 0;
    DirectedGraph g = Sealib::GraphCreator::imbalanced(200);
    uint64_t m = 0;
    for (uint64_t u = 0; u < g.getOrder(); u++) {
        for (uint64_t k = 0; k < g.deg(u); k++) {
            m++;
        }
    }
    DFS::nloglognBitDFS(g, incr1, incr2, incr3, incr4);
    EXPECT_EQ(c1, 200);
    EXPECT_EQ(c2, m);
    EXPECT_EQ(c3, m);
    EXPECT_EQ(c4, 200);
}

auto *graph = new uint64_t[19]{5,  9,  7, 9,  9, 7,  12, 1, 17, 2,
                               12, 14, 3, 14, 4, 12, 17, 5, 14};
uint64_t controllSum = (2 * (1 + 2 + 3 + 4 + 5));
stack<uint64_t> controllStack;
void preTwo(uint64_t a) {
    controllSum = controllSum - a;
    controllStack.push(a);
}
void postTwo(uint64_t a) {
    controllSum = controllSum - a;
    uint64_t ex = controllStack.top();
    controllStack.pop();
    EXPECT_EQ(ex, a);
}

TEST(DFSTest, inplace_dfs_all_of_grade_ge_2) {
    DFS::runLinearTimeInplaceDFS(graph, preTwo, postTwo, 1);
    EXPECT_EQ(0, controllSum);
}

}  // namespace Sealib
