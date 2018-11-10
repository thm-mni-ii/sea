#ifndef SEALIB_ITERATOR_DFS_H_
#define SEALIB_ITERATOR_DFS_H_

#include "sealib/_types.h"
#include "sealib/collection/compactarray.h"
#include "sealib/graph/graph.h"
#include "sealib/graph/node.h"
#include "sealib/graph/undirectedgraph.h"
#include "sealib/collection/staticspacestorage.h"

#define DFS_WHITE 0
#define DFS_GRAY 1
#define DFS_BLACK 2
#define DFS_RESERVED 3

typedef void (*UserFunc1)(uint);
typedef void (*UserFunc2)(uint, uint);

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
    static void standardDFS(Graph *g, UserFunc1 preprocess,
                            UserFunc2 preexplore, UserFunc2 postexplore,
                            UserFunc1 postprocess);
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
     * Run a linear-time space-efficient depth-first search. (Elmasry, Hagerup
    * and
     * Kammer; 2015) <br>
     * EFFICIENCY: O(n+m) time, O(n log log n) bits
    * @param g graph G=(V,E) to iterate over
    * @param preprocess to be executed before processing a node u
    * @param preexplore to be executed before exploring an edge (u,v)
    * @param postexplore to be executed after exploring an edge (u,v)
    * @param postprocess to be executed after processing a node u
    * @author Simon Heuser
    */
    static void nloglognBitDFS(Graph *g, UserFunc1 preprocess,
                               UserFunc2 preexplore, UserFunc2 postexplore,
                               UserFunc1 postprocess);

    /**
     * Run a linear-time and linear-space depth-first search over an undirected
     * graph. <br>
     * EFFICIENCY: O(n+m) time, O(n+m) bits
     * @param g undirected graph G=(V,E) to iterate over
     * @param preprocess to be executed before processing a node u
     * @param preexplore to be executed before exploring an edge (u,v)
     * @param postexplore to be executed after exploring an edge (u,v)
     * @param postprocess to be executed after processing a node u
     * @author Simon Heuser
     */
    static void nplusmBitDFS(UndirectedGraph *g, UserFunc1 preprocess,
                             UserFunc2 preexplore, UserFunc2 postexplore,
                             UserFunc1 postprocess);

    /**
     * Runs an inplace DFS in linear time over a graph that is given in a
     * special representation. <br>
     * EFFICIENCY: O(n+m) time, O(log n) bits
     * @param graph Graph A graph in a swapped begin pointer representation.
     * @param startVertex startVertex The begin of the DFS tree.
     * @author Simon Schniedenharn
     */
    static void runLinearTimeInplaceDFS(uint32_t *graph,
                                        UserFunc1 preProcess,
                                        UserFunc1 postProcess,
                                        uint32_t startVertex);
};
}  // namespace Sealib
#endif  // SEALIB_ITERATOR_DFS_H_
