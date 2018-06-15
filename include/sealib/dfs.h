#ifndef SEALIB_DFS_H_
#define SEALIB_DFS_H_

#include <stack>
#include <tuple>
#include "sealib/compactarray.h"
#include "sealib/graph.h"
#include "sealib/node.h"

 #define DFS_DEBUG

#define DFS_WHITE 0
#define DFS_GRAY 1
#define DFS_BLACK 2
#define DFS_NOP_PROCESS (void (*)(Node *))0
#define DFS_NOP_EXPLORE (void (*)(Node *, Node *))0

typedef void (*UserFunc1)(Node *);
typedef void (*UserFunc2)(Node *, Node *);
typedef unsigned int uint;

/**
 * This class contains depth-first search algorithms.
 * The depth-first search of a graph processes all its nodes and
 * explores all its edges.
 * During this procedure, the nodes will be colored
 * from white (initial) to gray (being processed) to black (finished processing)
 *
 * The following DFS variants are available
 *  - StandardDFS: the normal DFS, uses implicit recursion stack
 *
 * @author Simon Heuser
 */
class DFS {
 private:
 public:
  /**
   * Run a standard depth-first search over a graph. <br>
   * EFFICIENCY: O(n+m) time, O(n log n) bits
   * @param g graph G=(V,E) to iterate over
   * @param [pre|post][Process|Explore]: user-defined functions <br>
   * to be executed before/after processing a node/exploring an edge
   * @param preprocess to be executed before processing a node u
   * @param preexplore to be executed before exploring an edge (u,v)
   * @param postexplore to be executed after exploring an edge (u,v)
   * @param postprocess to be executed after processing a node u
   */
  static void runStandardDFS(Graph *g, UserFunc1 preprocess,
                             UserFunc2 preexplore, UserFunc2 postexplore,
                             UserFunc1 postprocess);

  static void runSmallDFS(Graph *g, UserFunc1 preprocess, UserFunc2 preexplore,
                          UserFunc2 postexplore, UserFunc1 postprocess);
};
#endif  // SEALIB_DFS_H_
