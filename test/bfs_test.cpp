#include "sealib/bfs.h"
#include <gtest/gtest.h>
#include <stdio.h>
#include "sealib/basicgraph.h"
#include "sealib/graphcreator.h"
using Sealib::BFS;
using Sealib::Graph;
using Sealib::Basicgraph;

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
    g.push_back(Sealib::GraphCreator::createRandomFixed(order, degree));
  }
  return g;
}

class BFSTest : public ::testing::TestWithParam<Graph *> {
 protected:
  virtual void SetUp() { c1 = c2 = 0; }
};

INSTANTIATE_TEST_CASE_P(ParamTests, BFSTest, ::testing::ValuesIn(makeGraphs()),
                        /**/);

TEST_P(BFSTest, userProc) {
  Graph *g = GetParam();
  BFS::run(g, incr1, incr2);
  ASSERT_EQ(c1, order);
  ASSERT_EQ(c2, order * degree);
  delete g;
}
