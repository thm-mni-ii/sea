#ifndef SEALIB_DFS_H_
#define SEALIB_DFS_H_

#include <stack>
#include <vector>
#include "sealib/_types.h"
#include "sealib/compactarray.h"
#include "sealib/graph.h"
#include "sealib/iterator.h"
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

  /**
   * Representation of a user call. A sequence of these can be retrieved from
   * the ReverseDFS iterator.
   */
  struct UserCall {
   public:
    enum Type { preprocess = 0, preexplore, postexplore, postprocess };
    unsigned type;
    uint u, v;
    UserCall(unsigned p1, uint p2, uint p3 = 0) : type(p1), u(p2), v(p3) {}
  };

  /**
   * Reverse DFS iterator.
   * - init():
   * - more():
   * - next():
   */
  class ReverseDFS : Iterator<UserCall> {
   public:
    explicit ReverseDFS(Graph *);
    ~ReverseDFS();

    /**
     * Run a n*log(log(n))-Bit DFS to record data about the intervals
     */
    void init() override;

    /**
     * @return true if there are more UserCalls in the DFS
     */
    bool more() override;

    /**
     * Get the next user call from the interval.
     * If necessary, first reconstruct the stack and simulate the DFS until the
     * end of the interval.
     * @return next user call from the reverse sequence
     */
    UserCall next() override;

   private:
    struct IntervalData {
     public:
      Pair h1, h2;   // top entries at start and end of the interval
      Pair hd;       // value of deepest entry
      int hdc = -1;  // index of deepest entry
      uint ic = 0;   // call counter for the interval
    };

    Graph *g;
    uint n, r, w;
    CompactArray *c;
    ExtendedSegmentStack *s;  // used for recording run
    CompactArray *d, *f;
    int ns = 0;
    IntervalData *i;
    uint ip = 0;  // interval pointer (a.k.a. j)
    std::vector<UserCall> seq;
    uint sp;

    void storeTime(unsigned df, uint u);
    void storeBeginTime(uint u);
    void storeEndTime(uint u);

    std::stack<Pair> reconstructPart(uint j, Pair from, Pair to);

    std::vector<UserCall> simulate(std::stack<Pair> sj, Pair until);
  };
};
}  // namespace Sealib
#endif  // SEALIB_DFS_H_
