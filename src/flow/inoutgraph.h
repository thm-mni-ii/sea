#ifndef SRC_FLOW_INOUTGRAPH_H_
#define SRC_FLOW_INOUTGRAPH_H_

#include <sealib/graph/graph.h>

namespace Sealib {
/**
 * The class InOutGraph class is a wrapper that
 * transforms a given graph into a network flow graph
 * for the computation of a separator.
 * @author Vytautas Hermann
 */
class InOutGraph : public Graph {
 public:
    explicit InOutGraph(Graph const &graph) : g(graph) {}
    uint64_t deg(uint64_t v) const override {
        if (v < g.getOrder())
            return 1;
        else
            return g.deg(v - g.getOrder());
    }
    uint64_t head(uint64_t u, uint64_t k) const override {
        if (u < g.getOrder())
            return u + g.getOrder();
        else
            return g.head(u - g.getOrder(), k);
    }
    uint64_t getOrder() const override { return g.getOrder() * 2; }

 private:
    Graph const &g;
};

}  // namespace Sealib

#endif  // SRC_FLOW_INOUTGRAPH_H_
