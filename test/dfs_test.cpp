#include "sealib/dfs.h"
#include <gtest/gtest.h>

#define order 30

Node *s[order * 2];
int sp = 0;
void pushN(Node *u) {
  printf("pushing %p\n", (void *)u);
  s[sp++] = u;
}

TEST(DFSTest, runStd) {
  Node *nodes = reinterpret_cast<Node *>(calloc(1, sizeof(Node) * 2 * order));
  for (unsigned int a = 0; a < order - 2; a++) {
    nodes[a] = Node(new Adjacency(a + 1), 1U);
  }
  nodes[order - 1] = Node(new Adjacency(4), 1U);
  DFS::runStandardDFS(new Graph(nodes, order), pushN, DFS_NOP_EXPLORE,
                      DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
  ASSERT_EQ(s[order - 1], nodes + (order - 1));
  while (sp-- > 0) {
    ASSERT_EQ(s[sp], nodes + sp);
  }
  free(nodes);
}
TEST(DFSTest, runSmall) {
  Node *nodes = reinterpret_cast<Node *>(calloc(1, sizeof(Node) * 2 * order));
  for (unsigned int a = 0; a < order - 1; a++) {
    nodes[a] = Node(new Adjacency(a + 1), 1U);
  }
  nodes[order - 1] = Node(new Adjacency(4), 1U);
  sp = 0;
  DFS::runEHKDFS(new Graph(nodes, order), DFS_NOP_PROCESS, DFS_NOP_EXPLORE,
                 DFS_NOP_EXPLORE, pushN);
  for (int a = 0; a < order; a++) {
    bool ok = false;
    for (int b = 0; b < sp; b++) {
      if (nodes + a == s[b]) {
        ok = true;
        break;
      }
    }
    printf("%u ", a);
    ASSERT_TRUE(ok);  // node a has been processed
  }
  free(nodes);
}
