#include <gtest/gtest.h>
#include <sealib/basicgraph.h>
#include <sealib/dfs.h>
#include <sealib/graphcreator.h>
#include <memory>
#include <random>
#include <stack>
#include <vector>
#include "sealib/basicgraph.h"
#include "sealib/graphcreator.h"
#include "sealib/reversedfs.h"
#include "sealib/runtimetest.h"

using Sealib::DFS;
using Sealib::CompactArray;
using Sealib::Graph;
using Sealib::BasicGraph;
using Sealib::Node;
using Sealib::Adjacency;
using std::vector;
using std::stack;

static unsigned c1 = 0, c2 = 0, c3 = 0, c4 = 0;
static unsigned tmp = 0;

static void incr1(uint u) {
  c1++;
  tmp = u;
}

static void incr4(uint u) {
  c4++;
  tmp = u;
}

static void incr2(uint u, uint v) {
  c2++;
  tmp = u;
  tmp = v;
}

static void incr3(uint u, uint v) {
  c3++;
  tmp = u;
  tmp = v;
}

/*void p0(uint u) { printf("preprocess %u\n", u); }
void p1(uint u) { printf("postprocess %u\n", u); }
void e0(uint u, uint v) { printf("preexplore %u,%u\n", u, v); }
void e1(uint u, uint v) { printf("postexplore %u,%u\n", u, v); }*/

static std::random_device rnd;
static const unsigned GRAPHCOUNT = 4;  // how many random graphs to generate?
static const unsigned DEGREE = 15;     // how many outneighbours per node?
static const unsigned ORDER = 200;

static std::vector<std::shared_ptr<Graph>> makeGraphs() {
  std::vector<std::shared_ptr<Graph>> g;
  for (uint c = 0; c < GRAPHCOUNT; c++) {
    g.push_back(std::shared_ptr<Graph>(
        Sealib::GraphCreator::createRandomFixed(ORDER, DEGREE)));
  }
  return g;
}

class DFSTest : public ::testing::TestWithParam<std::shared_ptr<Graph>> {
 protected:
  virtual void SetUp() {
    c1 = c2 = c3 = c4 = 0;
    v1 = std::vector<unsigned>(ORDER);
    v4 = std::vector<unsigned>(ORDER);
  }  // executed before each
     // TEST_P
};

INSTANTIATE_TEST_CASE_P(ParamTests, DFSTest, ::testing::ValuesIn(makeGraphs()),
                        /**/);

TEST_P(DFSTest, stdUserproc) {
  Graph *g = GetParam().get();
  DFS::standardDFS(g, incr1, incr2, incr3, incr4);
  EXPECT_EQ(c1, ORDER);
  for (unsigned a : v1) {
    EXPECT_EQ(a, 1);
  }
  EXPECT_EQ(c2, DEGREE * ORDER);
  EXPECT_EQ(c3, DEGREE * ORDER);
  EXPECT_EQ(c4, ORDER);
  for (unsigned a : v1) {
    EXPECT_EQ(a, 1);
  }
}

TEST_P(DFSTest, nBitUserproc) {
  Graph *g = GetParam().get();
  DFS::nBitDFS(g, incr1, incr2, incr3, incr4);
  EXPECT_EQ(c1, ORDER);
  for (unsigned a : v1) {
    EXPECT_EQ(a, 1);
  }
  EXPECT_EQ(c2, DEGREE * ORDER);
  EXPECT_EQ(c3, DEGREE * ORDER);
  EXPECT_EQ(c4, ORDER);
  for (unsigned a : v1) {
    EXPECT_EQ(a, 1);
  }
}

TEST_P(DFSTest, nloglognBitUserproc) {
  Graph *g = GetParam().get();
  DFS::nloglognBitDFS(g, incr1, incr2, incr3, incr4);
  EXPECT_EQ(c1, ORDER);
  for (unsigned a : v1) {
    EXPECT_EQ(a, 1);
  }
  EXPECT_EQ(c2, DEGREE * ORDER);
  EXPECT_EQ(c3, DEGREE * ORDER);
  EXPECT_EQ(c4, ORDER);
  for (unsigned a : v4) {
    EXPECT_EQ(a, 1);
  }
}

TEST(DFSTest, runtime) {
  uint deg = 5;
  RuntimeTest t1, t2;
  for (uint n = 10000; n < 1000000; n += 10000) {
    Graph *g = Sealib::GraphCreator::createRandomFixed(n, deg);
    // t1.runTest(
    //    [&g]() {
    //      DFS::nloglognBitDFS(g, DFS_NOP_PROCESS, DFS_NOP_EXPLORE,
    //                          DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
    //    },
    //    n, n * deg);
    t2.runTest(
        [&g]() {
          DFS::standardDFS(g, DFS_NOP_PROCESS, DFS_NOP_EXPLORE, DFS_NOP_EXPLORE,
                           DFS_NOP_PROCESS);
        },
        n, n * deg);
  }
  // t1.saveCSV("nloglogn-dfs.csv");
  t2.saveCSV("standard-dfs.csv");
}

TEST(DFSTest, standardImbalanced) {
  Graph *g = Sealib::GraphCreator::createRandomImbalanced(ORDER);
  DFS::standardDFS(g, DFS_NOP_PROCESS, DFS_NOP_EXPLORE, DFS_NOP_EXPLORE,
                   DFS_NOP_PROCESS);
  SUCCEED();
  delete g;
}

TEST(DFSTest, nBitImbalanced) {
  Graph *g = Sealib::GraphCreator::createRandomImbalanced(ORDER);
  DFS::nBitDFS(g, DFS_NOP_PROCESS, DFS_NOP_EXPLORE, DFS_NOP_EXPLORE,
               DFS_NOP_PROCESS);
  SUCCEED();
  delete g;
}

TEST(DFSTest, nloglognBitImbalanced) {
  Graph *g = Sealib::GraphCreator::createRandomImbalanced(ORDER);
  DFS::nloglognBitDFS(g, DFS_NOP_PROCESS, DFS_NOP_EXPLORE, DFS_NOP_EXPLORE,
                      DFS_NOP_PROCESS);
  SUCCEED();
  delete g;
}

auto *graph = new unsigned int[19]{5,  9,  7, 9,  9, 7,  12, 1, 17, 2,
                                   12, 14, 3, 14, 4, 12, 17, 5, 14};
unsigned int controllSum = (2 * (1 + 2 + 3 + 4 + 5));
stack<unsigned int> controllStack;
void preTwo(unsigned int a) {
  controllSum = controllSum - a;
  controllStack.push(a);
}
void postTwo(unsigned int a) {
  controllSum = controllSum - a;
  unsigned int ex = controllStack.top();
  controllStack.pop();
  EXPECT_EQ(ex, a);
}

TEST(DFSTest, inplace_dfs_all_of_grade_ge_2) {
  DFS::runLinearTimeInplaceDFS(graph, preTwo, postTwo, 1);
  EXPECT_EQ(0, controllSum);
}
