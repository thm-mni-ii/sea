#include "sealib/dfs.h"
#include <gtest/gtest.h>
#include <random>

unsigned c1 = 0, c2 = 0;
void incr1(Node *u) {
  c1++;
  u->getDegree();
}
void incr2(Node *u, Node *v) {
  c2++;
  u->getDegree();
  v->getDegree();
}
// TODO(shsr04): make user actions functions of unsigned
void incrCnt(unsigned u) { cnt[u]++; }
Graph *g;
std::random_device rnd;
unsigned order = rnd() % 50000;
unsigned *cnt;
class DFSTest : public ::testing::Test {
 protected:
  Node *n = reinterpret_cast<Node *>(malloc(sizeof(Node) * order));
  virtual void SetUp() {  // executed before each TEST_F
    c1 = 0;
    c2 = 0;
    cnt = new unsigned[order];
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
  DFS::runStandardDFS(g, DFS_NOP_PROCESS, DFS_NOP_EXPLORE, incr2, incr1);
  EXPECT_EQ(c1, order);
  EXPECT_EQ(c2, 5 * order);  // every node has 5 edges
}
TEST_F(DFSTest, EHK_pre) {
  DFS::runEHKDFS(g, incr1, incr2, DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
  EXPECT_EQ(c1, order);
  EXPECT_EQ(c2, 5 * order);  // every node has 5 edges
}
TEST_F(DFSTest, EHK_post) {
  DFS::runEHKDFS(g, DFS_NOP_PROCESS, DFS_NOP_EXPLORE, incr2, incr1);
  EXPECT_EQ(c1, order);
  EXPECT_EQ(c2, 5 * order);  // every node has 5 edges
}
