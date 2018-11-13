#ifndef SEALIB_EDGEMARKER_H_
#define SEALIB_EDGEMARKER_H_
#include "sealib/basicgraph.h"
#include "sealib/compactarray.h"
#include "sealib/dfs.h"
#include "sealib/rankselect.h"
#include "sealib/staticspacestorage.h"

namespace Sealib {
class EdgeMarker {
 public:
    explicit EdgeMarker(BasicGraph *g);

    void identifyEdges(CompactArray *color, StaticSpaceStorage *parent);

    void markTreeEdges(CompactArray *color, StaticSpaceStorage *parent);

    void setMark(uint u, uint k, uint8_t mark);

    /** Edge data: (4 bits)
     *      TTTP
     *  T: edge type (0: cross/forward edge, 1: back edge, 2: unmarked tree
     * edge, 3: half-marked tree edge, 4: full-marked tree edge)
     *  P: parent (0: further away from root, 1: closer to root)
     */
    const uint8_t TYPE_MASK = 0xe,  // 0b1110
        PARENT_MASK = 0x1;          // 0b0001
    const uint8_t FULL = 0x8, HALF = 0x6, UNMARKED = 0x4, BACK = 0x2,
                  CROSS = 0x0;
    const uint8_t PARENT = 0x1;

    CONSTEXPR_IF_CLANG bool isTreeEdge(uint u, uint k) const {
        return (getEdgeData(u, k) & TYPE_MASK) >= UNMARKED;
    }
    CONSTEXPR_IF_CLANG bool isParent(uint u, uint k) const {
        return (getEdgeData(u, k) & PARENT_MASK) == PARENT;
    }
    CONSTEXPR_IF_CLANG bool isFullMarked(uint u, uint k) const {
        return (getEdgeData(u, k) & TYPE_MASK) == FULL;
    }

    CONSTEXPR_IF_CLANG BasicGraph *getGraph() const { return g; }

 private:
    BasicGraph *g;
    uint n;
    StaticSpaceStorage edges;
    RankSelect offset;

    void initEdge(uint u, uint k, uint8_t type);

    void markParents(uint w, uint u, StaticSpaceStorage *parent);

    CONSTEXPR_IF_CLANG uint edgeIndex(uint u) const {
        return static_cast<uint>(offset.select(u + 1) - u - 1U);
    }
    CONSTEXPR_IF_CLANG uint64_t getEdgeData(uint u, uint k) const {
        return edges.get(edgeIndex(u) + k);
    }
};
}  // namespace Sealib
#endif  // SEALIB_EDGEMARKER_H_
