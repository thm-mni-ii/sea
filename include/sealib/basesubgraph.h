#ifndef SEALIB_BASESUBGRAPH_H_
#define SEALIB_BASESUBGRAPH_H_
#include <sealib/subgraph.h>
#include <sealib/rankselect.h>
#include <sealib/basicgraph.h>
#include <tuple>

namespace Sealib {
class BaseSubGraph : public SubGraph {
    typedef std::shared_ptr<BasicGraph> rgraph_t;
 private:
    rgraph_t rGraph;

    static bitset_t initializeQSelect(rgraph_t rGraph);
    static bitset_t initializePSelect(rgraph_t rGraph);
 public:
    explicit BaseSubGraph(std::shared_ptr<BasicGraph> rGraph);

    ~BaseSubGraph() override;

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
#endif  // SEALIB_BASESUBGRAPH_H_
