#ifndef SEALIB_RECURSIVESUBGRAPH_H_
#define SEALIB_RECURSIVESUBGRAPH_H_
#include <sealib/subgraph.h>
#include <sealib/rankselect.h>

namespace Sealib {
class RecursiveSubGraph : public SubGraph {
    typedef SubGraph rgraph_t;
 private:
    rgraph_t *rGraph;
    rankselect_t vSelect;
    rankselect_t aSelect;

    bitset_t initializeQSelect();
    bitset_t initializePSelect();

 public:
    RecursiveSubGraph(SubGraph *rGraph_,
                     Sealib::Bitset<unsigned char> &v,
                     Sealib::Bitset<unsigned char> &e);

    ~RecursiveSubGraph() override;

    unsigned long degree(unsigned long v) const override;
    unsigned long head(unsigned long u, unsigned long k) const override;
    std::tuple<unsigned long, unsigned long> mate(unsigned long u, unsigned long k) const override;
    unsigned long order() const override;
    unsigned long arcNumber(unsigned long j, unsigned long k) const override;
    std::tuple<unsigned long, unsigned long> inverseArcNumber(unsigned long r) const override;
    unsigned long translateVertex(unsigned long u) const override;
    unsigned long translateArc(unsigned long e) const override;
};
}  // namespace Sealib
#endif  // SEALIB_RECURSIVESUBGRAPH_H_
