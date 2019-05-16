#ifndef SEALIB_ITERATOR_OUTERPLANARCHECKER_H_
#define SEALIB_ITERATOR_OUTERPLANARCHECKER_H_
#include <set>
#include <vector>
#include "sealib/_types.h"
#include "sealib/collection/compactarray.h"
#include "sealib/dictionary/raggeddictionary.h"
#include "sealib/dictionary/rankselect.h"
#include "sealib/graph/undirectedgraph.h"
#include "sealib/graph/virtualgraph.h"

namespace Sealib {
/**
 * Recognizes if a given biconnected graph is outerplanar.
 * (Note: You can use the BCC iterator to make a biconnected subgraph from an
 * arbitrary graph.)
 * EFFICIENCY: O(n*log(log(n))) time, O(n) bits
 *
 * @author Simon Heuser
 */
class OuterplanarChecker {
 public:
    /**
     * Create the outerplanarity checker.
     * @param g biconnected graph
     */
    explicit OuterplanarChecker(UndirectedGraph const& g);

    /**
     * Run the algorithm to check if the given biconnected graph G is
     * outerplanar.
     * @return true if G is outerplanar, false otherwise
     */
    bool isOuterplanar();

    uint64_t byteSize() const {
        return tried.capacity() / 8 + paths.byteSize() + pathOffset.byteSize() +
               g.byteSize();
    }

 private:
    VirtualGraph g;
    uint64_t n, m;
    ChoiceDictionary d;
    ChoiceDictionaryIterator di;
    std::vector<bool> tried;
    CompactArray paths;
    RankSelect pathOffset;

    bool removeClosedChains();
    bool removeAllChains();

    bool incrementPaths(uint64_t u, uint64_t k);

    /**
     * Data referring to a chain. A chain is a path where each vertex has degree
     * 2 except two endpoints which have degree > 2.
     */
    struct ChainData {
        std::pair<uint64_t, uint64_t>
            c1,  ///< endpoint u1 + outgoing edge k1 pointing into the chain
            c2;  ///< endpoint u2 + outgoing edge k2 pointing into the chain
        bool isClosed =
                 false,      ///< true if the endpoints are connected by an edge
            isGood = false,  ///< true if one of the endpoints has degree < 4
            isCycle = false,  ///< true if the chain has only degree 2 vertices
            isTried = false;  ///< true if an inner vertex is tried
    };

    /**
     * Walk through the chain that contains u.
     * @param u Vertex in the interior of the chain
     * @param phase2 called from phase 2? (unlocks artificial edges)
     * @return data collected when iterating over the chain
     */
    [[gnu::hot]] ChainData chain(uint64_t u, bool phase2 = true);
    bool removeChain(ChainData const& c, bool phase2 = true);

    void forEdge(uint64_t v1, uint64_t v2,
                 std::function<void(uint64_t, uint64_t)> f);

    [[gnu::hot]] void forEach(
        ChainData const& c, Consumer v,
        BiConsumer e = [](uint64_t, uint64_t) {});
};
}  // namespace Sealib

#endif  // SEALIB_ITERATOR_OUTERPLANARCHECKER_H_
