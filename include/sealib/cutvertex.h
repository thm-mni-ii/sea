#ifndef SEALIB_CUTVERTEX_H_
#define SEALIB_CUTVERTEX_H_
#include <vector>
#include "sealib/_types.h"
#include "sealib/basicgraph.h"
#include "sealib/bitset.h"
#include "sealib/choicedictionaryiterator.h"
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

    bool isCutVertex(uint u);

 private:
    BasicGraph *g;
    uint n;
    RankSelect offset;
    StaticSpaceStorage edges;
    ChoiceDictionary cc;
    ChoiceDictionary cut;
    ChoiceDictionaryIterator cutI;

    void setTreeEdge(uint u, uint k, bool uChild);
    void setMark(uint u, uint k, uint8_t mark);
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
    const uint8_t TREE = 0x8;
    const uint8_t CHILD = 0x4;
    const uint8_t FULL = 0x2, HALF = 0x1;
    CONSTEXPR_IF_CLANG bool isTreeEdge(uint u, uint k) const {
        return (getEdgeData(u, k) & EDGE_BIT) == TREE;
    }
    CONSTEXPR_IF_CLANG bool isParent(uint u, uint k) const {
        return (getEdgeData(u, k) & DIRECTION_BIT) == 0;
    }
    CONSTEXPR_IF_CLANG bool isFullMarked(uint u, uint k) const {
        return (getEdgeData(u, k) & MARKING_BITS) == FULL;
    }

    CONSTEXPR_IF_CLANG uint edgeIndex(uint u) const {
        return static_cast<uint>(offset.select(u + 1) - u - 1U);
    }
    CONSTEXPR_IF_CLANG uint64_t getEdgeData(uint u, uint k) const {
        return edges.get(edgeIndex(u) + k);
    }
};
}  // namespace Sealib
#endif  // SEALIB_CUTVERTEX_H_
