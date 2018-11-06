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
    StaticSpaceStorage edges;
    std::vector<uint> offs{0};

    inline uint edgeIndex(uint u, uint v) const {
        for (uint k = 0; k < g->getNodeDegree(u); k++) {
            if (g->head(u, k) == v) {
                return k;
            }
        }
        return INVALID;
    }

    CONSTEXPR_IF_CLANG inline uint64_t getEdgeData(uint u, uint v) const {
        return edges.get(offs[u] + edgeIndex(u, v));
    }
};
}  // namespace Sealib
#endif  // SEALIB_CUTVERTEX_H_
