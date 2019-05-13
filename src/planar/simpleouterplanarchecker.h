#ifndef SRC_PLANAR_SIMPLEOUTERPLANARCHECKER_H_
#define SRC_PLANAR_SIMPLEOUTERPLANARCHECKER_H_
#include <queue>
#include <vector>
#include "sealib/_types.h"
#include "sealib/graph/undirectedgraph.h"
#include "../src/graph/simplevirtualgraph.h"

namespace Sealib {
/**
 * Simple implementation that checks if a biconnected graph is outerplanar. It
 * works by repeatedly removing vertices of degree 2. If necessary, the
 * neighbours of the removed vertex are connected using a virtual edge.
 * (Wiegers; 1986)
 */
class SimpleOuterplanarChecker {
 public:
    /**
     * Create a new object that checks the given biconnected graph for
     * outerplanarity.
     * @param g biconnected undirected graph G=(V,E)
     */
    explicit SimpleOuterplanarChecker(UndirectedGraph const &g);

    /**
     * Run the check for outerplanarity.
     * @return true if the given graph is outerplanar, false otherwise
     */
    bool isOuterplanar();

    uint64_t byteSize() const {
        uint64_t nOuter = 0;
        for (std::vector<bool> const &v : isOuter) {
            nOuter += v.capacity();
        }
        return g.byteSize() + dmax * sizeof(uint64_t) + nOuter / 8;
    }

 private:
    SimpleVirtualGraph g;
    uint64_t n, m;
    std::queue<uint64_t> degTwo;
    uint64_t dmax = 0;
    std::vector<std::vector<bool>> isOuter;

    bool edgeBetween(uint64_t u, uint64_t v);
};
}  // namespace Sealib
#endif  // SRC_PLANAR_SIMPLEOUTERPLANARCHECKER_H_
