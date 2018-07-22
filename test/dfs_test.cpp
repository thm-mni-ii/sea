#include "sealib/dfs.h"
#include <gtest/gtest.h>
#include <random>

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
    c1 = c2 = c3 = c4 = 0;
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
  DFS::runStandardDFS(g, incr1, incr2, incr3, incr4);
  // DFS::runStandardDFS(g, p0, e0, e1, p1);
  EXPECT_EQ(c1, order);
  EXPECT_EQ(c2, 5 * order);  // every node has 5 edges
  EXPECT_EQ(c3, 5 * order);
  EXPECT_EQ(c4, order);
}
// TODO(!!!): fix random test failures!
TEST_F(DFSTest, EHK_userproc) {
  unsigned errorcount = 0;
  for (int a = 0; a < 10; a++) {
    c1 = c2 = c3 = c4 = 0;
    DFS::runEHKDFS(g, incr1, incr2, incr3, incr4);
    if (c1 != order) {
      printf("c1 = %u\n", c1);
      errorcount++;
    } else if (c2 != 5 * order) {
      printf("c2 = %u\n", c2);
      errorcount++;  // every node has 5 edges
    } else if (c3 != 5 * order) {
      printf("c3 = %u\n", c3);
      errorcount++;
    } else if (c4 != order) {
      printf("c4 = %u\n", c4);
      errorcount++;
    }
  }
  EXPECT_EQ(errorcount, 0) << "failed in " << errorcount
                           << " out of 10 iterations";
}
