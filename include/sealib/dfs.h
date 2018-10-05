#ifndef SEALIB_DFS_H_
#define SEALIB_DFS_H_

#include <utility>
#include "sealib/_types.h"
#include "sealib/compactarray.h"
#include "sealib/graph.h"
#include "sealib/node.h"
#include "sealib/segmentstack.h"

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
#define DFS_NOP_PROCESS [](uint) {}        // NOLINT
#define DFS_NOP_EXPLORE [](uint, uint) {}  // NOLINT

namespace Sealib {
/**
 * This class contains depth-first search algorithms.
 * The depth-first search of a graph processes all its nodes and
 * explores all its edges.
 * During this procedure, the nodes will be colored
 * from white (initial) to gray (being processed) to black (finished processing)
 *
 * The following DFS variants are available:
 *  - standardDFS: the normal DFS, uses implicit recursion stack
 *  - nBitDFS: space-efficient DFS, explicit segmented stack
 *
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
   * @author Simon Heuser
   */
  static void standardDFS(Graph *g, UserFunc1 preprocess, UserFunc2 preexplore,
                          UserFunc2 postexplore, UserFunc1 postprocess);
  /**
   * Run a space-efficient depth-first search over a given graph. (Elmasry,
   * Hagerup and Kammer; 2015) <br>
   * EFFICIENCY: O((n+m) log n) time, O((log3 + ε) n) bits
   * @param g graph G=(V,E) to iterate over
   * @param preprocess to be executed before processing a node u
   * @param preexplore to be executed before exploring an edge (u,v)
   * @param postexplore to be executed after exploring an edge (u,v)
   * @param postprocess to be executed after processing a node u
   * @author Simon Heuser
   */
  static void nBitDFS(Graph *g, UserFunc1 preprocess, UserFunc2 preexplore,
                      UserFunc2 postexplore, UserFunc1 postprocess);

  /**
   * Run a linear-time space-efficient depth-first search. (Elmasry, Hagerup and
   * Kammer; 2015) <br>
   * EFFICIENCY: O(n+m) time, O(n log log n) bits
  * @param g graph G=(V,E) to iterate over
  * @param preprocess to be executed before processing a node u
  * @param preexplore to be executed before exploring an edge (u,v)
  * @param postexplore to be executed after exploring an edge (u,v)
  * @param postprocess to be executed after processing a node u
  */
  static void nloglognBitDFS(Graph *g, UserFunc1 preprocess = DFS_NOP_PROCESS,
                             UserFunc2 preexplore = DFS_NOP_EXPLORE,
                             UserFunc2 postexplore = DFS_NOP_EXPLORE,
                             UserFunc1 postprocess = DFS_NOP_PROCESS);

  /**
   * Runs an inplace DFS in linear time over a graph that is given in a special
   * representation. <br>
   * EFFICIENCY: O(n+m) time, O(log n) bits
   * @param graph Graph A graph in a swapped begin pointer representation.
   * @param startVertex startVertex The begin of the DFS tree.
   * @author Simon Schniedenharn
   */
  static void runLinearTimeInplaceDFS(unsigned int *graph, UserFunc1 preProcess,
                                      UserFunc1 postProcess,
                                      unsigned int startVertex);

#ifdef VISUALIZE

 protected:
#else

 private:
#endif
  static void process_standard(uint u0, Graph *g, uint *color,
                               UserFunc1 preProcess, UserFunc2 preExplore,
                               UserFunc2 postExplore, UserFunc1 postProcess);

  template <class SS>
  static void process_small(uint u0, Graph *g, CompactArray *color, SS *s,
                            void (*restoration)(uint, Graph *, CompactArray *,
                                                SS *),
                            UserFunc1 preProcess, UserFunc2 preExplore,
                            UserFunc2 postExplore, UserFunc1 postProcess);

  static void restore_full(uint u0, Graph *g, CompactArray *color,
                           BasicSegmentStack *s);

  static std::pair<bool, uint> findEdge(const uint u, const uint k, Graph *g,
                                        CompactArray *c,
                                        ExtendedSegmentStack *s);
  static void restore_top(uint u0, Graph *g, CompactArray *color,
                          ExtendedSegmentStack *s);
};
}  // namespace Sealib
#endif  // SEALIB_DFS_H_
