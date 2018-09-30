#ifndef SEALIB_RECURSIVESUBGRAPH_H_
#define SEALIB_RECURSIVESUBGRAPH_H_
#include <sealib/subgraph.h>
#include <sealib/rankselect.h>
#include <tuple>

namespace Sealib {
class RecursiveSubGraph : public SubGraph {
    typedef SubGraph rgraph_t;

 private:
    rgraph_t *rGraph;
    rankselect_t vSelect;
    rankselect_t aSelect;

    bitset_t initializeQSelect();
    bitset_t initializePSelect();

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
    RecursiveSubGraph(SubGraph *rGraph_,
                      const Sealib::Bitset<unsigned char> &v,
                      const Sealib::Bitset<unsigned char> &e);

    ~RecursiveSubGraph() override;

    unsigned long degree(unsigned long u) const override;
    unsigned long head(unsigned long u, unsigned long k) const override;
    std::tuple<unsigned long, unsigned long> mate(unsigned long u, unsigned long k) const override;

    unsigned long phi(unsigned long u) const override;
    unsigned long psi(unsigned long a) const override;
    unsigned long phiInv(unsigned long u) const override;
    unsigned long psiInv(unsigned long a) const override;
};
}  // namespace Sealib
#endif  // SEALIB_RECURSIVESUBGRAPH_H_
