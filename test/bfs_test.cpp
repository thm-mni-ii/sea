#include "sealib/bfs.h"
#include <gtest/gtest.h>
#include <stdio.h>
#include "sealib/graph.h"
#include "sealib/graphcreator.h"

using Sealib::BFS;
using Sealib::ConnectedComponent;
using Sealib::Graph;
using Sealib::GraphCreator;

static uint c1 = 0, c2 = 0;
static uint tmp = 0;
static void incr1(uint u) {
  c1++;
  tmp = u;
}
static void incr2(uint u, uint v) {
  c2++;
  tmp = u;
  tmp = v;
}

static uint GRAPHCOUNT = 4, order = 500, degree = 20;
static std::vector<Graph *> makeGraphs() {
  std::vector<Graph *> g = std::vector<Graph *>();
  for (uint c = 0; c < GRAPHCOUNT; c++) {
    g.push_back(GraphCreator::createRandomFixed(order, degree));
  }
  return g;
}

class BFSTest : public ::testing::TestWithParam<Graph *> {
 protected:
  virtual void SetUp() { c1 = c2 = 0; }
};

INSTANTIATE_TEST_CASE_P(ParamTests, BFSTest, ::testing::ValuesIn(makeGraphs()),
                        /**/);

TEST_P(BFSTest, userproc) {
  Graph *g = GetParam();
  BFS::run(g, incr1, incr2);
  EXPECT_EQ(c1, order);
  EXPECT_EQ(c2, order * degree);
  delete g;
}

TEST(BFSTest, connectedComponents1) {
  Graph *g = GraphCreator::createRandomFixed(order, 0);
  std::vector<ConnectedComponent> c =
      BFS::run(g, BFS_NOP_PROCESS, BFS_NOP_EXPLORE);
  ASSERT_EQ(c.size(), order);
  uint b = 0;
  for (ConnectedComponent a : c) {
    EXPECT_EQ(a.get(0).head(), b);
    b++;
  }
  delete g;
}

TEST(BFSTest, connectedComponents2) {
  Graph *g = GraphCreator::createRandomFixed(order, 1);
  std::vector<ConnectedComponent> c =
      BFS::run(g, BFS_NOP_PROCESS, BFS_NOP_EXPLORE);
  std::set<uint> u;
  for (ConnectedComponent a : c) {
    for (Pair b : a.getSequence()) {
      std::pair<std::set<uint>::iterator, bool> r = u.insert(b.head());
      EXPECT_TRUE(std::get<1>(r));
    }
  }
  delete g;
}
