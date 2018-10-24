#ifndef SRC_RECURSIVESUBGRAPH_H_
#define SRC_RECURSIVESUBGRAPH_H_
#include <sealib/rankselect.h>
#include <tuple>
#include "./subgraph.h"

namespace Sealib {
class RecursiveSubGraph : public SubGraph {
 protected:
    rankselect_t vSelect;
    rankselect_t aSelect;

    bitset_t initializeVSelect(const bitset_t &v);
    bitset_t initializeASelect(const bitset_t &v);

    inline unsigned long select_v(unsigned long i) const {
        return vSelect.select(i);
    }

    inline unsigned long select_a(unsigned long i) const {
        return aSelect.select(i);
    }

    inline unsigned long rank_v(unsigned long i) const {
        return vSelect.rank(i);
    }

    inline unsigned long rank_a(unsigned long i) const {
        return aSelect.rank(i);
    }

 public:
    RecursiveSubGraph(stack_t *stack,
                      unsigned long sidx_,
                      unsigned long ridx_,
                      const Sealib::Bitset<unsigned char> &v,
                      const Sealib::Bitset<unsigned char> &a);

    unsigned long head(unsigned long u, unsigned long k) const override;
    std::tuple<unsigned long, unsigned long> mate(unsigned long u, unsigned long k) const override;

    unsigned long phi(unsigned long u) const override;
    unsigned long psi(unsigned long a) const override;
    unsigned long phiInv(unsigned long u) const override;
    unsigned long psiInv(unsigned long a) const override;
    ~RecursiveSubGraph() override;
};
}  // namespace Sealib
#endif  // SRC_RECURSIVESUBGRAPH_H_
