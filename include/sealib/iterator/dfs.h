#ifndef SEALIB_ITERATOR_DFS_H_
#define SEALIB_ITERATOR_DFS_H_

#include "sealib/_types.h"
#include "sealib/collection/compactarray.h"
#include "sealib/collection/staticspacestorage.h"
#include "sealib/graph/graph.h"
#include "sealib/graph/node.h"
#include "sealib/graph/undirectedgraph.h"
#include "sealib/segmentstack.h"

namespace Sealib {

const uint DFS_WHITE = 0, DFS_GRAY = 1, DFS_BLACK = 2, DFS_RESERVED = 3;
const Consumer DFS_NOP_PROCESS = [](uint) {};
const BiConsumer DFS_NOP_EXPLORE = [](uint, uint) {};

/**
 * This class contains depth-first search algorithms.
 * The depth-first search of a graph processes all its nodes and
 * explores all its edges.
 * During this procedure, the nodes will be colored
 * from white (initial) to gray (being processed) to black (finished processing)
 *
 * The following DFS variants are available:
 *  - standardDFS: the normal DFS, uses implicit recursion stack
 *  - nBitDFS: slow but very space-efficient DFS, explicit segmented stack
 *  - nloglognBitDFS: fast and space-efficient DFS, segment storage
 *  - nplusmBitDFS: good space-efficient DFS for undirected graphs, static space
 * allocation
 *  - runLinearTimeInplaceDFS: space-efficient DFS over a compact graph
 */
class DFS {
 public:
    /**
     * Run a standard depth-first search over a given graph.
     * EFFICIENCY: O(n+m) time, O(n log n) bits
     * @param g graph G=(V,E) to iterate over
     * @param preprocess to be executed before processing a node u
     * @param preexplore to be executed before exploring an edge (u,v)
     * @param postexplore to be executed after exploring an edge (u,v)
     * @param postprocess to be executed after processing a node u
     * @author Simon Heuser
     */
    static void standardDFS(Graph *g, Consumer preprocess = DFS_NOP_PROCESS,
                            BiConsumer preexplore = DFS_NOP_EXPLORE,
                            BiConsumer postexplore = DFS_NOP_EXPLORE,
                            Consumer postprocess = DFS_NOP_PROCESS);
    /**
     * Run a space-efficient depth-first search over a given graph. (Elmasry,
     * Hagerup and Kammer; 2015)
     * EFFICIENCY: O((n+m) log n) time, O((log3 + ε) n) bits
     * @param g graph G=(V,E) to iterate over
     * @param preprocess to be executed before processing a node u
     * @param preexplore to be executed before exploring an edge (u,v)
     * @param postexplore to be executed after exploring an edge (u,v)
     * @param postprocess to be executed after processing a node u
     * @author Simon Heuser
     */
    static void nBitDFS(Graph *g, Consumer preprocess = DFS_NOP_PROCESS,
                        BiConsumer preexplore = DFS_NOP_EXPLORE,
                        BiConsumer postexplore = DFS_NOP_EXPLORE,
                        Consumer postprocess = DFS_NOP_PROCESS);

    /**
     * Run a linear-time space-efficient depth-first search. (Elmasry, Hagerup
     * and Kammer; 2015)
     * EFFICIENCY: O(n+m) time, O(n log log n) bits
     * @param g graph G=(V,E) to iterate over
     * @param preprocess to be executed before processing a node u
     * @param preexplore to be executed before exploring an edge (u,v)
     * @param postexplore to be executed after exploring an edge (u,v)
     * @param postprocess to be executed after processing a node u
     * @author Simon Heuser
     */
    static void nloglognBitDFS(Graph *g, Consumer preprocess = DFS_NOP_PROCESS,
                               BiConsumer preexplore = DFS_NOP_EXPLORE,
                               BiConsumer postexplore = DFS_NOP_EXPLORE,
                               Consumer postprocess = DFS_NOP_PROCESS);

    /**
     * Run a linear-time and linear-space depth-first search over an undirected
     * graph. The preexplore and postexplore calls can take (u,k) as argument
     * due to the nature of an undirected graph.
     * EFFICIENCY: O(n+m) time, O(n+m) bits
     * @param g undirected graph G=(V,E) to iterate over
     * @param preprocess to be executed before processing a node u
     * @param preexplore to be executed before exploring an edge (u,k)
     * @param postexplore to be executed after exploring an edge (u,k)
     * @param postprocess to be executed after processing a node u
     * @author Simon Heuser
     */
    static void nplusmBitDFS(UndirectedGraph *g,
                             Consumer preprocess = DFS_NOP_PROCESS,
                             BiConsumer preexplore = DFS_NOP_EXPLORE,
                             BiConsumer postexplore = DFS_NOP_EXPLORE,
                             Consumer postprocess = DFS_NOP_PROCESS);

    /**
     * Runs an inplace DFS in linear time over a graph that is given in a
     * special representation. <br>
     * EFFICIENCY: O(n+m) time, O(log n) bits
     * @param graph Graph A graph in a swapped begin pointer representation.
     * @param startVertex startVertex The begin of the DFS tree.
     * @author Simon Schniedenharn
     */
    static void runLinearTimeInplaceDFS(uint32_t *graph, Consumer preProcess,
                                        Consumer postProcess,
                                        uint32_t startVertex);

 protected:
    static void process_standard(uint u0, Graph *g, uint *color,
                                 Consumer preprocess, BiConsumer preexplore,
                                 BiConsumer postexplore, Consumer postprocess);

    template <class SS>
    static void process_small(uint u0, Graph *g, CompactArray *color, SS *s,
                              void (*restoration)(uint, Graph *, CompactArray *,
                                                  SS *),
                              Consumer preprocess, BiConsumer preexplore,
                              BiConsumer postexplore, Consumer postprocess);

    template <class S>
    static void process_static(uint u0, UndirectedGraph *g, CompactArray *color,
                               S *back, Consumer preprocess,
                               BiConsumer preexplore, BiConsumer postexplore,
                               Consumer postprocess);

    static void restore_full(uint u0, Graph *g, CompactArray *color,
                             BasicSegmentStack *s);

    static void restore_top(uint u0, Graph *g, CompactArray *color,
                            ExtendedSegmentStack *s);

#ifdef SEALIB_ITERATOR_EDGEMARKER_H_
    friend class EdgeMarker;
#endif
};
}  // namespace Sealib
#endif  // SEALIB_ITERATOR_DFS_H_
