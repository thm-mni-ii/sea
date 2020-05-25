#ifndef SRC_SUBGRAPH_RECURSIVESUBGRAPH_H_
#define SRC_SUBGRAPH_RECURSIVESUBGRAPH_H_
#include <sealib/dictionary/rankselect.h>
#include <tuple>
#include "./subgraph.h"

namespace Sealib {
class RecursiveSubGraph : public SubGraph {
 protected:
    rankselect_t vSelect;
    rankselect_t aSelect;

    bitset_t initializeVSelect(const bitset_t &v);
    bitset_t initializeASelect(const bitset_t &v);

    inline uint64_t select_v(uint64_t i) {
        return vSelect.select(i);
    }

    inline uint64_t select_a(uint64_t i) {
        return aSelect.select(i);
    }

    inline uint64_t rank_v(uint64_t i) {
        return vSelect.rank(i);
    }

    inline uint64_t rank_a(uint64_t i) {
        return aSelect.rank(i);
    }

 public:
    RecursiveSubGraph(stack_t *stack,
                      uint64_t sidx_,
                      uint64_t ridx_,
                      const bitset_t &v,
                      const bitset_t &a);

    RecursiveSubGraph(stack_t *stack,
                      uint64_t sidx_,
                      uint64_t ridx_,
                      bitset_t &&v,
                      bitset_t &&a);

    uint64_t head(uint64_t u, uint64_t k) override;
    std::tuple<uint64_t, uint64_t> mate(uint64_t u, uint64_t k) override;

    /**
     * For the following functions:
     * @throws ZeroArgumentGiven if u or a is 0
     */
    uint64_t phi(uint64_t u) override;
    uint64_t psi(uint64_t a) override;
    uint64_t phiInv(uint64_t u) final;
    uint64_t psiInv(uint64_t a) final;

    ~RecursiveSubGraph() override;
};
}  // namespace Sealib
#endif  // SRC_SUBGRAPH_RECURSIVESUBGRAPH_H_
