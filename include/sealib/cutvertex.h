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

    inline void init_tree(CompactArray *, StaticSpaceStorage *);
    inline void init_mark(CompactArray *, StaticSpaceStorage *);
    inline void init_cut();

    void initEdgeType(uint u, uint k, uint8_t type);
    void setMark(uint u, uint k, uint8_t mark);
    void markParents(uint w, uint u, StaticSpaceStorage *parent);

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

    CONSTEXPR_IF_CLANG uint edgeIndex(uint u) const {
        return static_cast<uint>(offset.select(u + 1) - u - 1U);
    }
    CONSTEXPR_IF_CLANG uint64_t getEdgeData(uint u, uint k) const {
        return edges.get(edgeIndex(u) + k);
    }
};
}  // namespace Sealib
#endif  // SEALIB_CUTVERTEX_H_
