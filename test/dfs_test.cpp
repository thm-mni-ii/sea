#include "sealib/dfs.h"
#include <gtest/gtest.h>
#include <random>
#include <stack>
#include <vector>
#include "sealib/basicgraph.h"
#include "sealib/graphcreator.h"

using Sealib::DFS;
using Sealib::CompactArray;
using Sealib::Graph;
using Sealib::Basicgraph;
using Sealib::Node;
using Sealib::Adjacency;
using std::vector;
using std::stack;

unsigned c1 = 0, c2 = 0, c3 = 0, c4 = 0;
unsigned tmp = 0;

void incr1(uint u) {
  c1++;
  tmp = u;
}

void incr4(uint u) {
  c4++;
  tmp = u;
}

void incr2(uint u, uint v) {
  c2++;
  tmp = u;
  tmp = v;
}

void incr3(uint u, uint v) {
  c3++;
  tmp = u;
  tmp = v;
}

void p0(uint u) { printf("preprocess %u\n", u); }
void p1(uint u) { printf("postprocess %u\n", u); }
void e0(uint u, uint v) { printf("preexplore %u,%u\n", u, v); }
void e1(uint u, uint v) { printf("postexplore %u,%u\n", u, v); }

std::random_device rnd;
const unsigned GRAPHCOUNT = 10;  // how many random graphs to generate?
const unsigned DEGREE = 5;       // how many outneighbours per node?
const unsigned order = 200;

std::vector<Graph *> makeGraphs() {
  std::vector<Graph *> g = std::vector<Graph *>();
  for (uint c = 0; c < GRAPHCOUNT; c++) {
    g.push_back(Sealib::GraphCreator::createRandomFixed(order, DEGREE));
  }
  return g;
}

std::vector<Graph *> graphs = makeGraphs();
class DFSTest : public ::testing::TestWithParam<Graph *> {
 protected:
  virtual void SetUp() { c1 = c2 = c3 = c4 = 0; }  // executed before each
                                                   // TEST_P
};

INSTANTIATE_TEST_CASE_P(ParamTests, DFSTest, ::testing::ValuesIn(graphs), /**/);

TEST_P(DFSTest, stdUserproc) {
  Graph *g = GetParam();
  DFS::standardDFS(g, incr1, incr2, incr3, incr4);
  EXPECT_EQ(c1, order);
  EXPECT_EQ(c2, DEGREE * order);
  EXPECT_EQ(c3, DEGREE * order);
  EXPECT_EQ(c4, order);
}

TEST_P(DFSTest, nBitUserproc) {
  Graph *g = GetParam();
  DFS::nBitDFS(g, incr1, incr2, incr3, incr4);
  EXPECT_EQ(c1, order);
  EXPECT_EQ(c2, DEGREE * order);
  EXPECT_EQ(c3, DEGREE * order);
  EXPECT_EQ(c4, order);
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
