#ifndef SEALIB_ITERATOR_DFS_H_
#define SEALIB_ITERATOR_DFS_H_
#include <functional>
#include <vector>
#include "sealib/_types.h"
#include "sealib/collection/compactarray.h"
#include "sealib/collection/segmentstack.h"
#include "sealib/collection/staticspacestorage.h"
#include "sealib/graph/directedgraph.h"
#include "sealib/graph/undirectedgraph.h"

namespace Sealib {

static const uint64_t DFS_WHITE = 0, DFS_GRAY = 1, DFS_BLACK = 2,
                      DFS_RESERVED = 3;
static Consumer DFS_NOP_PROCESS = [](uint64_t) {};
static BiConsumer DFS_NOP_EXPLORE = [](uint64_t, uint64_t) {};

/**
 * This class contains depth-first search algorithms.
 * The depth-first search of a graph processes all its nodes and explores all
 * its edges.
 * During this procedure, the nodes will be colored from white (initial) to gray
 * (being processed) to black (finished processing).
 * You can give user calls (aka callbacks) to the DFS procedures that will be
 * called at appropriate times:
 * - preprocess(u): before a vertex u turns gray
 * - postprocess(u): after a vertex u turns black
 * - preexplore(u,k): before the k'th edge outgoing from u is considered
 * - postexplore(u,k): after the k'th edge outgoing from u is considered
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
     * @param preexplore to be executed before exploring an edge (u,k)
     * @param postexplore to be executed after exploring an edge (u,k)
     * @param postprocess to be executed after processing a node u
     * @author Simon Heuser
     */
    static void standardDFS(Graph const &g,
                            Consumer preprocess = DFS_NOP_PROCESS,
                            BiConsumer preexplore = DFS_NOP_EXPLORE,
                            BiConsumer postexplore = DFS_NOP_EXPLORE,
                            Consumer postprocess = DFS_NOP_PROCESS);
    /**
     * Run a space-efficient depth-first search over a given graph. (Elmasry,
     * Hagerup and Kammer; 2015)
     * EFFICIENCY: O((n+m) log n) time, O((log3 + ε) n) bits
     * @param g graph G=(V,E) to iterate over
     * @param preprocess to be executed before processing a node u
     * @param preexplore to be executed before exploring an edge (u,k)
     * @param postexplore to be executed after exploring an edge (u,k)
     * @param postprocess to be executed after processing a node u
     * @author Simon Heuser
     */
    static void nBitDFS(Graph const &g, Consumer preprocess = DFS_NOP_PROCESS,
                        BiConsumer preexplore = DFS_NOP_EXPLORE,
                        BiConsumer postexplore = DFS_NOP_EXPLORE,
                        Consumer postprocess = DFS_NOP_PROCESS);

    /**
     * Run a linear-time space-efficient depth-first search. (Elmasry, Hagerup
     * and Kammer; 2015)
     * EFFICIENCY: O(n+m) time, O(n log log n) bits
     * @param g graph G=(V,E) to iterate over
     * @param preprocess to be executed before processing a node u
     * @param preexplore to be executed before exploring an edge (u,k)
     * @param postexplore to be executed after exploring an edge (u,k)
     * @param postprocess to be executed after processing a node u
     * @author Simon Heuser
     */
    static void nloglognBitDFS(Graph const &g,
                               Consumer preprocess = DFS_NOP_PROCESS,
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
    static void nplusmBitDFS(UndirectedGraph const &g,
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
    static void runLinearTimeInplaceDFS(uint64_t *graph, Consumer preProcess,
                                        Consumer postProcess,
                                        uint64_t startVertex);

    /**
     * The following helper procedures are only for internal or experimental
     * usage.
     */

    static void visit_standard(uint64_t u0, Graph const &g,
                               std::vector<uint64_t> *color,
                               std::vector<std::pair<uint64_t, uint64_t>> *s,
                               Consumer preProcess, BiConsumer preExplore,
                               BiConsumer postExplore, Consumer postProcess);

    static void visit_nloglogn(
        uint64_t u0, Graph const &g, CompactArray *color, SegmentStack *s,
        std::function<void(uint64_t, Graph const &, CompactArray *,
                           SegmentStack *)>
            restoration,
        Consumer preprocess, BiConsumer preexplore, BiConsumer postexplore,
        Consumer postprocess);

    static void visit_nplusm(uint64_t u0, UndirectedGraph const &g,
                             CompactArray *color, Sequence<uint64_t> *parent,
                             Consumer preprocess, BiConsumer preexplore,
                             BiConsumer postexplore, Consumer postprocess);

    static void restore_full(uint64_t u0, Graph const &g, CompactArray *color,
                             /*Basic*/ SegmentStack *s);

    static void restore_top(uint64_t u0, Graph const &g, CompactArray *color,
                            /*Extended*/ SegmentStack *s);
};
}  // namespace Sealib
#endif  // SEALIB_ITERATOR_DFS_H_
