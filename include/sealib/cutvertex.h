#ifndef SEALIB_CUTVERTEX_H_
#define SEALIB_CUTVERTEX_H_
#include <vector>
#include "sealib/_types.h"
#include "sealib/basicgraph.h"
#include "sealib/compactarray.h"
#include "sealib/dfs.h"
#include "sealib/iterator.h"

namespace Sealib {
class CutVertexIterator : Iterator<uint>, DFS {
 public:
    explicit CutVertexIterator(BasicGraph *g);

    void init() override;

    bool more() override;

    uint next() override;

 private:
    BasicGraph *g;
    uint n;
    std::vector<uint> offs;
    StaticSpaceStorage edges;
    std::vector<uint> cc{0};
    uint cci = 0;
    uint cu = 0;

    void setTreeEdge(uint u, uint v);
    void setMark(uint u, uint v, uint8_t mark);
    void markParents(uint w, uint u, StaticSpaceStorage *parent);

    /** Edge data: (4 bits)
     *      EDMM
     *  E: edge type (0: back edge, 1: tree edge)
     *  D: "direction" (0: closer to root, 1: further away from root)
     *  M: marking (0: unmarked, 1: half marked, 2: full marked)
     */
    const uint8_t EDGE_BIT = 0x8,  // 0b1000
        DIRECTION_BIT = 0x4,       // 0b0100
        MARKING_BITS = 0x3;        // 0b0011
    const uint8_t TREE = 1;
    const uint8_t FULL = 2, HALF = 1;
    CONSTEXPR_IF_CLANG bool isTreeEdge(uint u, uint v) const {
        return (getEdgeData(u, v) & EDGE_BIT) == TREE;
    }
    CONSTEXPR_IF_CLANG bool isParent(uint u, uint v) const {
        return getEdgeData(u, v) & DIRECTION_BIT;
    }
    CONSTEXPR_IF_CLANG bool isFullMarked(uint u, uint v) const {
        return (getEdgeData(u, v) & MARKING_BITS) == FULL;
    }

    inline uint edgeIndex(uint u, uint v) const {
        for (uint k = 0; k < g->getNodeDegree(u); k++) {
            if (g->head(u, k) == v) {
                return k;
            }
        }
        return INVALID;
    }
    CONSTEXPR_IF_CLANG uint64_t getEdgeData(uint u, uint v) const {
        return edges.get(offs[u] + edgeIndex(u, v));
    }
};
}  // namespace Sealib
#endif  // SEALIB_CUTVERTEX_H_
