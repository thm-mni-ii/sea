#include "sealib/dfs.h"
#include <gtest/gtest.h>

#define MAX_STACK_SIZE 500

Node *s[MAX_STACK_SIZE];
int sp = 0;
void pushN(Node *u) {
  // printf("pushing %p\n", reinterpret_cast<void *>(u));
  s[sp++] = u;
}
void printI(Node *u) {
  static unsigned i = 0;
  printf("%u\n", i++);
  u->getDegree();
}

TEST(DFSTest, runStd) {
  unsigned order = 30;
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
  unsigned order = 100;
  Node *nodes = reinterpret_cast<Node *>(malloc(sizeof(Node) * order));
  for (unsigned int a = 0; a < order - 1; a++) {
    nodes[a] = Node(new Adjacency(a + 1), 1U);
  }
  nodes[order - 1] = Node(new Adjacency(4), 1U);
  sp = 0;
  DFS::runEHKDFS(new Graph(nodes, order), DFS_NOP_PROCESS, DFS_NOP_EXPLORE,
                 DFS_NOP_EXPLORE, printI);
  /*for (int a = 0; a < order; a++) {
    bool ok = false;
    for (int b = 0; b < sp; b++) {
      if (nodes + a == s[b]) {
        ok = true;
        break;
      }
    }
    printf("%u ", a);
    ASSERT_TRUE(ok);  // node a has been processed
  }*/
  free(nodes);
}
