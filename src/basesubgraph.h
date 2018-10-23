#ifndef SEALIB_BASESUBGRAPH_H_
#define SEALIB_BASESUBGRAPH_H_
#include <src/subgraph.h>
#include <sealib/rankselect.h>
#include <sealib/basicgraph.h>
#include <tuple>

namespace Sealib {
class BaseSubGraph : public SubGraph {
    typedef std::shared_ptr<BasicGraph> rgraph_t;
 private:
    rgraph_t rGraph;

 public:
    unsigned long head(unsigned long u, unsigned long k) const override;

    std::tuple<unsigned long, unsigned long> mate(unsigned long u, unsigned long k) const override;

    unsigned long phi(unsigned long u) const override;
    unsigned long psi(unsigned long a) const override;
    unsigned long phiInv(unsigned long u) const override;
    unsigned long psiInv(unsigned long a) const override;
    explicit BaseSubGraph(stack_t *stack, std::shared_ptr<BasicGraph> rGraph);
    ~BaseSubGraph() override;
};
}  // namespace Sealib
#endif  // SEALIB_BASESUBGRAPH_H_
