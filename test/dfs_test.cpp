#include "sealib/dfs.h"
#include <gtest/gtest.h>
#include <random>

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
unsigned *cnt;
void incrCnt(unsigned u) { cnt[u]++; }
Graph *g;
std::random_device rnd;
unsigned order = 0;
class DFSTest : public ::testing::Test {
 protected:
  Node *n;
  virtual void SetUp() {  // executed before each TEST_F
    order = rnd() % 50000;
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
  EXPECT_EQ(c1, 2 * order);
  EXPECT_EQ(c2, 2 * 5 * order);  // every node has 5 edges
}
TEST_F(DFSTest, EHK_pre) {
  DFS::runEHKDFS(g, incr1, incr2, DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
  EXPECT_EQ(c1, order);
  EXPECT_EQ(c2, 5 * order);  // every node has 5 edges
}
TEST_F(DFSTest, EHK_post) {
  try {
    DFS::runEHKDFS(g, DFS_NOP_PROCESS, DFS_NOP_EXPLORE, incr2, incr1);
  } catch (unsigned e) {
    printf("caught %u\n", e);
  }
  /*bool ok = true;
  for (unsigned a = 0; a < order; a++) {
    if (cnt[a] != 1) {
      printf("a: %u, cnt: %u\n", a, cnt[a]);
      ok = false;
    }
  }
  EXPECT_TRUE(ok);*/
  EXPECT_EQ(c1, order);
  EXPECT_EQ(c2, 5 * order);  // every node has 5 edges
  delete cnt;
}
