#ifndef SEALIB_ITERATOR_EDGEMARKER_H_
#define SEALIB_ITERATOR_EDGEMARKER_H_
#include "sealib/collection/compactarray.h"
#include "sealib/collection/staticspacestorage.h"
#include "sealib/dictionary/rankselect.h"
#include "sealib/graph/undirectedgraph.h"
#include "sealib/iterator/dfs.h"

#ifdef SEALIBVISUAL_EXAMPLES_H_
namespace SealibVisual {
class VisualEdgeMarker;
}
#endif

namespace Sealib {
/**
 * The EdgeMarker class holds edge markings for a given graph. An edge has
 * markings if it is a tree edge. Tree edges are identified using a DFS, and
 * markings for edges are set using another DFS run. With the edge markings, you
 * can run algorithms like cut-vertex finding and biconnected-component
 * outputting.
 */
class EdgeMarker : DFS {
 public:
    /**
     * Create a new edge marker from a given undirected graph. On construction,
     * it automatically identifies edge types and marks tree edges.
     * @param g undirected graph
     */
    explicit EdgeMarker(UndirectedGraph const *g);

    /**
     * Get the graph that this edge marker is using.
     * @return pointer to the undirected graph used
     */
    CONSTEXPR_IF_CLANG UndirectedGraph const *getGraph() const { return g; }

    CONSTEXPR_IF_CLANG bool isInitialized(uint u, uint k) const {
        return (getEdgeData(u, k) & TYPE_MASK) != NONE;
    }
    CONSTEXPR_IF_CLANG bool isTreeEdge(uint u, uint k) const {
        return (getEdgeData(u, k) & TYPE_MASK) >= UNMARKED;
    }
    CONSTEXPR_IF_CLANG bool isBackEdge(uint u, uint k) const {
        return (getEdgeData(u, k) & TYPE_MASK) == BACK;
    }
    /**
     * @return true if u is closer to the root of the DFS tree
     */
    CONSTEXPR_IF_CLANG bool isParent(uint u, uint k) const {
        return (getEdgeData(u, k) & PARENT_MASK) == PARENT;
    }
    CONSTEXPR_IF_CLANG bool isFullMarked(uint u, uint k) const {
        return (getEdgeData(u, k) & TYPE_MASK) == FULL;
    }

    virtual ~EdgeMarker() = default;

 protected:
    /**
     * Set the marking on the kth edge of u (and on the jth edge of the mate v).
     * @param u node to modify an edge for
     * @param k outgoing edge index
     * @param mark marking (can be FULL, HALF or UNMARKED)
     */
    virtual void setMark(uint u, uint k, uint8_t mark);

    /**
     * Initializes the kth edge of u to the given type. u will be set as parent.
     * @param u node to modify type for
     * @param k outgoing edge index
     * @param type FULL, HALF, UNMARKED, BACK or CROSS
     */
    virtual void initEdge(uint u, uint k, uint8_t type);

 private:
    /**
     * Runs a DFS to classify edges of G. When an outging edge (u,k) points to a
     * white node: tree edge; to a gray node: back edge; to a black node:
     * cross/forward edge.
     */
    void identifyEdges();

    /**
     * Runs a DFS to mark descendant back edges of each node. For each back edge
     * {u,w}, we chain upwards from w until we reach a full-marked edge or u
     * itself.
     */
    void markTreeEdges();

    /** Edge data: (4 bits)
     *      TTTP
     *  T: edge type (0: uninitialized, 1: cross/forward edge, 2: back edge, 3:
     * unmarked tree
     * edge, 4: half-marked tree edge, 5: full-marked tree edge)
     *  P: parent (0: further away from root, 1: closer to root)
     */
    static const uint8_t TYPE_MASK = 0xe,  // 0b1110
        PARENT_MASK = 0x1;                 // 0b0001
    static const uint8_t FULL = 0xa, HALF = 0x8, UNMARKED = 0x6, BACK = 0x4,
                         CROSS = 0x2, NONE = 0x0;
    static const uint8_t PARENT = 0x1;

    UndirectedGraph const *g;
    uint n;
    StaticSpaceStorage parent;
    StaticSpaceStorage edges;
    RankSelect offset;

    void markParents(uint w, uint u);

    CONSTEXPR_IF_CLANG uint edgeIndex(uint u) const {
        return static_cast<uint>(offset.select(u + 1) - u - 1U);
    }
    CONSTEXPR_IF_CLANG uint64_t getEdgeData(uint u, uint k) const {
        return edges.get(edgeIndex(u) + k);
    }

#ifdef SEALIBVISUAL_EXAMPLES_H_
    friend class SealibVisual::VisualEdgeMarker;
#endif
};
}  // namespace Sealib
#endif  // SEALIB_ITERATOR_EDGEMARKER_H_
