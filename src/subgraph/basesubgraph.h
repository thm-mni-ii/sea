#ifndef SRC_SUBGRAPH_BASESUBGRAPH_H_
#define SRC_SUBGRAPH_BASESUBGRAPH_H_
#include <sealib/dictionary/rankselect.h>
#include <sealib/graph/undirectedgraph.h>
#include <tuple>
#include "./subgraph.h"

namespace Sealib {
class BaseSubGraph : public SubGraph {
    typedef std::shared_ptr<UndirectedGraph> rgraph_t;
 private:
    rgraph_t rGraph;

 public:
    uint64_t head(uint64_t u, uint64_t k) override;

    std::tuple<uint64_t, uint64_t> mate(uint64_t u, uint64_t k) override;

    /**
     * For the following functions:
     * @throws ZeroArgumentGiven if u or a is 0
     */
    uint64_t phi(uint64_t u) override;
    uint64_t psi(uint64_t a) override;
    uint64_t phiInv(uint64_t u) override;
    uint64_t psiInv(uint64_t a) override;

    explicit BaseSubGraph(stack_t *stack, std::shared_ptr<UndirectedGraph> rGraph);
    ~BaseSubGraph() override;
};
}  // namespace Sealib
#endif  // SRC_SUBGRAPH_BASESUBGRAPH_H_
