#include "sealib/dfs.h"
#include <gtest/gtest.h>
#include <random>
#include <vector>

using Sealib::DFS;
using Sealib::CompactArray;
using Sealib::Graph;
using Sealib::Node;
using Sealib::Adjacency;

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
    Node *n = reinterpret_cast<Node *>(malloc(sizeof(Node) * order));
    for (unsigned int a = 0; a < order; a++) {
      int ai = DEGREE;
      Adjacency *ad =
          reinterpret_cast<Adjacency *>(malloc(sizeof(Adjacency) * ai));
      for (int b = 0; b < ai; b++) ad[b] = Adjacency(rnd() % order);
      n[a] = Node(ad, ai);
    }
    g.push_back(new Graph(n, order));
  }
  return g;
}
std::vector<Graph *> graphs = makeGraphs();
class DFSTest : public ::testing::TestWithParam<Graph *> {
 protected:
  virtual void SetUp() { c1 = c2 = c3 = c4 = 0; }  // executed before each
                                                   // TEST_P
};
INSTANTIATE_TEST_CASE_P(ParamTests, DFSTest, ::testing::ValuesIn(graphs));

TEST_P(DFSTest, StdUserproc) {
  Graph *g = GetParam();
  DFS::runStandardDFS(g, incr1, incr2, incr3, incr4);
  EXPECT_EQ(c1, order);
  EXPECT_EQ(c2, DEGREE * order);
  EXPECT_EQ(c3, DEGREE * order);
  EXPECT_EQ(c4, order);
}
TEST_P(DFSTest, EHKUserproc) {
  Graph *g = GetParam();
  DFS::runEHKDFS(g, incr1, incr2, incr3, incr4);
  EXPECT_EQ(c1, order);
  EXPECT_EQ(c2, DEGREE * order);
  EXPECT_EQ(c3, DEGREE * order);
  EXPECT_EQ(c4, order);
}
