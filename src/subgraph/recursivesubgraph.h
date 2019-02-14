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

    inline uint64_t select_v(uint64_t i) const {
        return vSelect.select(i);
    }

    inline uint64_t select_a(uint64_t i) const {
        return aSelect.select(i);
    }

    inline uint64_t rank_v(uint64_t i) const {
        return vSelect.rank(i);
    }

    inline uint64_t rank_a(uint64_t i) const {
        return aSelect.rank(i);
    }

 public:
    RecursiveSubGraph(stack_t *stack,
                      uint64_t sidx_,
                      uint64_t ridx_,
                      const Sealib::Bitset<uint8_t> &v,
                      const Sealib::Bitset<uint8_t> &a);

    uint64_t head(uint64_t u, uint64_t k) const override;
    std::tuple<uint64_t, uint64_t> mate(uint64_t u, uint64_t k) const override;

    uint64_t phi(uint64_t u) const override;
    uint64_t psi(uint64_t a) const override;
    uint64_t phiInv(uint64_t u) const final;
    uint64_t psiInv(uint64_t a) const final;
    ~RecursiveSubGraph() override;
};
}  // namespace Sealib
#endif  // SRC_SUBGRAPH_RECURSIVESUBGRAPH_H_
