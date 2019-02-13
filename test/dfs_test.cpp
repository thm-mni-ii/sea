#include "sealib/iterator/dfs.h"
#include <gtest/gtest.h>
#include <memory>
#include <random>
#include <stack>
#include <vector>
#include "sealib/graph/graphcreator.h"
#include "sealib/graph/undirectedgraph.h"

using std::vector;
using std::stack;

namespace Sealib {

static uint64_t c1 = 0, c2 = 0, c3 = 0, c4 = 0;

static void incr1(uint64_t) { c1++; }

static void incr4(uint64_t) { c4++; }

static void incr2(uint64_t, uint64_t) { c2++; }

static void incr3(uint64_t, uint64_t) { c3++; }

/*void p0(uint64_t u) { printf("preprocess %u\n", u); }
void p1(uint64_t u) { printf("postprocess %u\n", u); }
void e0(uint64_t u, uint64_t v) { printf("preexplore %u,%u\n", u, v); }
void e1(uint64_t u, uint64_t v) { printf("postexplore %u,%u\n", u, v); }*/

static std::random_device rnd;
static const uint64_t GRAPHCOUNT = 4;  // how many random graphs to generate?
static const uint64_t DEGREE = 15;     // how many outneighbours per node?
static const uint64_t order = 200;

static std::vector<DirectedGraph> makeGraphs() {
    std::vector<DirectedGraph> g;
    for (uint64_t c = 0; c < GRAPHCOUNT; c++) {
        g.push_back(Sealib::GraphCreator::kOutdegree(order, DEGREE));
    }
    return g;
}

class DFSTest : public ::testing::TestWithParam<DirectedGraph> {
 protected:
    virtual void SetUp() { c1 = c2 = c3 = c4 = 0; }  // executed before each
                                                     // TEST_P
};

INSTANTIATE_TEST_CASE_P(ParamTests, DFSTest, ::testing::ValuesIn(makeGraphs()),
                        /**/);

TEST_P(DFSTest, stdUserproc) {
    DirectedGraph g = GetParam();
    DFS::standardDFS(&g, incr1, incr2, incr3, incr4);
    EXPECT_EQ(c1, order);
    EXPECT_EQ(c2, DEGREE * order);
    EXPECT_EQ(c3, DEGREE * order);
    EXPECT_EQ(c4, order);
}

TEST_P(DFSTest, nBitUserproc) {
    DirectedGraph g = GetParam();
    DFS::nBitDFS(&g, incr1, incr2, incr3, incr4);
    EXPECT_EQ(c1, order);
    EXPECT_EQ(c2, DEGREE * order);
    EXPECT_EQ(c3, DEGREE * order);
    EXPECT_EQ(c4, order);
}

TEST_P(DFSTest, nloglognBitUserproc) {
    DirectedGraph g = GetParam();
    DFS::nloglognBitDFS(&g, incr1, incr2, incr3, incr4);
    EXPECT_EQ(c1, order);
    EXPECT_EQ(c2, DEGREE * order);
    EXPECT_EQ(c3, DEGREE * order);
    EXPECT_EQ(c4, order);
}

TEST(DFSTest, nplusmBitUserproc) {
    c1 = c2 = c3 = c4 = 0;
    uint64_t n = 4000;
    UndirectedGraph g = GraphCreator::kRegular(n, 20);
    DFS::nplusmBitDFS(&g, incr1, incr2, incr3, incr4);
    EXPECT_EQ(c1, n);
    EXPECT_EQ(c2, n * 20);
    EXPECT_EQ(c3, n * 20);
    EXPECT_EQ(c4, n);
}

TEST(DFSTest, nloglognImbalanced) {
    DirectedGraph g = Sealib::GraphCreator::imbalanced(order);
    DFS::nloglognBitDFS(&g, DFS_NOP_PROCESS, DFS_NOP_EXPLORE, DFS_NOP_EXPLORE,
                        DFS_NOP_PROCESS);
    SUCCEED();
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
