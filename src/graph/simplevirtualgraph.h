#ifndef SRC_GRAPH_SIMPLEVIRTUALGRAPH_H_
#define SRC_GRAPH_SIMPLEVIRTUALGRAPH_H_
#include "sealib/graph/undirectedgraph.h"

namespace Sealib {
class SimpleVirtualGraph : Graph {
 public:
    explicit SimpleVirtualGraph(UndirectedGraph const &g);

    uint64_t deg(uint64_t u) const override;

    uint64_t head(uint64_t u, uint64_t k) const override;

    uint64_t getOrder() const override;

    uint64_t index(uint64_t u, uint64_t v) const;

    void addEdge(uint64_t u, uint64_t v);

    void removeVertex(uint64_t u);

    uint64_t byteSize() const {
        uint64_t nAdj = 0;
        for (std::vector<uint64_t> const &v : virtualAdj) {
            nAdj += v.capacity();
        }
        return present.capacity() / 8 + nAdj * sizeof(uint64_t);
    }

 private:
    UndirectedGraph const &g;
    uint64_t n;
    std::vector<bool> present;
    std::vector<std::vector<uint64_t>> virtualAdj;
};
}  // namespace Sealib
#endif  // SRC_GRAPH_SIMPLEVIRTUALGRAPH_H_
