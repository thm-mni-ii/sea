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

static uint32_t c1 = 0, c2 = 0, c3 = 0, c4 = 0;

static void incr1(uint) {
    c1++;
}

static void incr4(uint) {
    c4++;
}

static void incr2(uint, uint) {
    c2++;
}

static void incr3(uint, uint) {
    c3++;
}

/*void p0(uint u) { printf("preprocess %u\n", u); }
void p1(uint u) { printf("postprocess %u\n", u); }
void e0(uint u, uint v) { printf("preexplore %u,%u\n", u, v); }
void e1(uint u, uint v) { printf("postexplore %u,%u\n", u, v); }*/

static std::random_device rnd;
static const uint32_t GRAPHCOUNT = 4;  // how many random graphs to generate?
static const uint32_t DEGREE = 15;     // how many outneighbours per node?
static const uint32_t order = 200;

static std::vector<DirectedGraph> makeGraphs() {
    std::vector<DirectedGraph> g;
    for (uint c = 0; c < GRAPHCOUNT; c++) {
        g.push_back(Sealib::GraphCreator::createRandomKRegularGraph(order, DEGREE));
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
    uint n = 4000;
    auto r = GraphCreator::createRandomUndirected(n, 20);
    UndirectedGraph *g = r.first;
    uint m = r.second;
    DFS::nplusmBitDFS(g, incr1, incr2, incr3, incr4);
    EXPECT_EQ(c1, n);
    EXPECT_EQ(c2, m);
    EXPECT_EQ(c3, m);
    EXPECT_EQ(c4, n);
}

TEST(DFSTest, nloglognImbalanced) {
    DirectedGraph g = Sealib::GraphCreator::createRandomImbalanced(order);
    DFS::nloglognBitDFS(&g, DFS_NOP_PROCESS, DFS_NOP_EXPLORE, DFS_NOP_EXPLORE,
                        DFS_NOP_PROCESS);
    SUCCEED();
}

auto *graph = new uint32_t[19]{5,  9,  7, 9,  9, 7,  12, 1, 17, 2,
                               12, 14, 3, 14, 4, 12, 17, 5, 14};
uint32_t controllSum = (2 * (1 + 2 + 3 + 4 + 5));
stack<uint32_t> controllStack;
void preTwo(uint32_t a) {
    controllSum = controllSum - a;
    controllStack.push(a);
}
void postTwo(uint32_t a) {
    controllSum = controllSum - a;
    uint32_t ex = controllStack.top();
    controllStack.pop();
    EXPECT_EQ(ex, a);
}

TEST(DFSTest, inplace_dfs_all_of_grade_ge_2) {
    DFS::runLinearTimeInplaceDFS(graph, preTwo, postTwo, 1);
    EXPECT_EQ(0, controllSum);
}

}  // namespace Sealib
