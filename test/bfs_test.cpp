#include "sealib/iterator/bfs.h"
#include <gtest/gtest.h>
#include <stdio.h>
#include "sealib/graph/graph.h"
#include "sealib/graph/graphcreator.h"

using Sealib::BFS;
using Sealib::BasicGraph;
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
static std::vector<BasicGraph> makeGraphs() {
  std::vector<BasicGraph> g;
  for (uint c = 0; c < GRAPHCOUNT; c++) {
    g.emplace_back(GraphCreator::createRandomFixed(order, degree));
  }
  return g;
}

class BFSTest : public ::testing::TestWithParam<BasicGraph> {
 protected:
  virtual void SetUp() { c1 = c2 = 0; }
};

INSTANTIATE_TEST_CASE_P(ParamTests, BFSTest, ::testing::ValuesIn(makeGraphs()),
                        /**/);

TEST_P(BFSTest, userproc) {
  BasicGraph g = GetParam();
  BFS bfs(&g, incr1, incr2);
  bfs.init();
  do {
    while (bfs.more()) bfs.next();
  } while (bfs.nextComponent());
  EXPECT_EQ(c1, order);
  EXPECT_EQ(c2, order * degree);
}

TEST(BFSTest, nextComponent) {
  c1 = c2 = 0;
  BasicGraph g = GraphCreator::createRandomFixed(order, 0);
  BFS bfs(&g, incr1, incr2);
  uint rc = 0;
  bfs.init();
  do {
    rc++;
    while (bfs.more()) bfs.next();
  } while (bfs.nextComponent());
  EXPECT_EQ(rc, order);
  EXPECT_EQ(c1, order);
  EXPECT_EQ(c2, 0);
}
