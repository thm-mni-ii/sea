#ifndef SEALIB_DFS_H_
#define SEALIB_DFS_H_

#include <stack>
#include <tuple>
#include "sealib/compactarray.h"
#include "sealib/graph.h"
#include "sealib/node.h"

typedef unsigned int uint;
typedef void (*UserFunc1)(uint);
typedef void (*UserFunc2)(uint, uint);

#define DFS_WHITE 0
#define DFS_GRAY 1
#define DFS_BLACK 2
#define DFS_RESERVED 3

/**
 * These two functions symbolize a NOP: you can call a DFS which accepts
 * (preprocess,preexplore,postexplore,postprocess) with the arguments
 * (DFS_NOP_PROCESS,DFS_NOP_EXPLORE,DFS_NOP_EXPLORE,DFS_NOP_PROCESS) to run it
 * silently.
 */
#define DFS_NOP_PROCESS (UserFunc1)0
#define DFS_NOP_EXPLORE (UserFunc2)0

namespace Sealib {
/**
 * This class contains depth-first search algorithms.
 * The depth-first search of a graph processes all its nodes and
 * explores all its edges.
 * During this procedure, the nodes will be colored
 * from white (initial) to gray (being processed) to black (finished processing)
 *
 * The following DFS variants are available:
 *  - StandardDFS: the normal DFS, uses implicit recursion stack
 *  - EHKDFS: space-efficient DFS, explicit stack
 *
 * @author Simon Heuser
 */
class DFS {
 public:
  /**
   * Run a standard depth-first search over a given graph. <br>
   * EFFICIENCY: O(n+m) time, O(n log n) bits
   * @param g graph G=(V,E) to iterate over
   * @param preprocess to be executed before processing a node u
   * @param preexplore to be executed before exploring an edge (u,v)
   * @param postexplore to be executed after exploring an edge (u,v)
   * @param postprocess to be executed after processing a node u
   */
  static void runStandardDFS(Graph *g, UserFunc1 preprocess,
                             UserFunc2 preexplore, UserFunc2 postexplore,
                             UserFunc1 postprocess);
  /**
   * Run a space-efficient depth-first search over a given graph. (Elmasry,
   * Hagerup and Kammer; 2015) <br>
   * EFFICIENCY: O((n+m) log n) time, O((log3 + ε) n) space
   * @param g graph G=(V,E) to iterate over
   * @param preprocess to be executed before processing a node u
   * @param preexplore to be executed before exploring an edge (u,v)
   * @param postexplore to be executed after exploring an edge (u,v)
   * @param postprocess to be executed after processing a node u
   */
  static void runEHKDFS(Graph *g, UserFunc1 preprocess, UserFunc2 preexplore,
                        UserFunc2 postexplore, UserFunc1 postprocess);
};
}  // namespace Sealib
#endif  // SEALIB_DFS_H_
