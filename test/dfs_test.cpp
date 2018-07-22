#include "sealib/dfs.h"
#include <gtest/gtest.h>
#include <random>

using Sealib::DFS;
using Sealib::CompactArray;
using Sealib::Graph;
using Sealib::Node;
using Sealib::Adjacency;

unsigned c1 = 0, c2 = 0;
unsigned tmp = 0;
void incr1(uint u) {
  c1++;
  tmp = u;
}
void incr2(uint u, uint v) {
  c2++;
  tmp = u;
  tmp = v;
}
void p0(uint u) { printf("preprocess %u\n", u); }
void p1(uint u) { printf("postprocess %u\n", u); }
void e0(uint u, uint v) { printf("preexplore %u,%u\n", u, v); }
void e1(uint u, uint v) { printf("postexplore %u,%u\n", u, v); }
unsigned *cnt;
void incrCnt(unsigned u) { cnt[u]++; }
Graph *g;
std::random_device rnd;
unsigned order = 0;
class DFSTest : public ::testing::Test {
 protected:
  Node *n;
  virtual void SetUp() {  // executed before each TEST_F
    order = 500;
    n = reinterpret_cast<Node *>(malloc(sizeof(Node) * order));
    c1 = 0;
    c2 = 0;
    for (unsigned int a = 0; a < order; a++) {
      int ai = 5;
      Adjacency *ad =
          reinterpret_cast<Adjacency *>(malloc(sizeof(Adjacency) * ai));
      for (int b = 0; b < ai; b++) ad[b] = Adjacency(rnd() % order);
      n[a] = Node(ad, ai);
    }
    g = new Graph(n, order);
  }
  virtual void TearDown() { free(n); }
};
TEST_F(DFSTest, runStd) {
  DFS::runStandardDFS(g, incr1, incr2, incr2, incr1);
  // DFS::runStandardDFS(g, p0, e0, e1, p1);
  EXPECT_EQ(c1, 2 * order);
  EXPECT_EQ(c2, 2 * 5 * order);  // every node has 5 edges
}
// TODO(!!!): fix random test failures!
TEST_F(DFSTest, EHKpre) {
  unsigned b = 0;
  for (int a = 0; a < 10; a++) {
    c1 = 0;
    c2 = 0;
    DFS::runEHKDFS(g, incr1, incr2, DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
    if (c1 != order)
      b++;
    else if (c2 != 5 * order)
      b++;  // every node has 5 edges
  }
  EXPECT_EQ(b, 0) << "failed in " << b << " out of 10 iterations";
}
TEST_F(DFSTest, EHKpost) {
  unsigned b = 0;
  for (int a = 0; a < 10; a++) {
    c1 = 0;
    c2 = 0;
    DFS::runEHKDFS(g, DFS_NOP_PROCESS, DFS_NOP_EXPLORE, incr2, incr1);
    if (c1 != order)
      b++;
    else if (c2 != 5 * order)
      b++;  // every node has 5 edges
  }
  EXPECT_EQ(b, 0) << "failed in " << b << " out of 10 iterations";
}
