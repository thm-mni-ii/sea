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
    explicit InOutGraph(
        Graph const &graph,
        const std::vector<std::pair<bool, std::pair<uint8_t, uint8_t>>> &paths)
        : g(graph), p(paths) {
        isrevert = false;
    }
    uint64_t deg(uint64_t v) const override {
        if (v < g.getOrder())
            return 1;
        else
            return g.deg(v - g.getOrder());
    }
    uint64_t head(uint64_t u, uint64_t k) const override {
        if (p[u].second.first != 0 && isrevert) {
            uint64_t x = 0;
            if (u < g.getOrder())
                x = u + g.getOrder();
            else
                x = g.head(u - g.getOrder(), k);
            if (p[u].second.second == p[x].second.first) {
                if (x < g.getOrder()) {
                    return u - g.getOrder();
                } else {
                    for (uint64_t i = 0; i < deg(u); i++) {
                        if ((p[g.head(u, i) + g.getOrder()].second.first) ==
                            p[u].second.second)
                            return g.head(u, i) + g.getOrder();
                    }
                    return u;
                }
            }
        }
        if (u < g.getOrder())
            return u + g.getOrder();
        else
            return g.head(u - g.getOrder(), k);
    }
    uint64_t getOrder() const override { return g.getOrder() * 2; }
    void setisrevert(bool x) { isrevert = x; }

 private:
    Graph const &g;
    std::vector<std::pair<bool, std::pair<uint8_t, uint8_t>>> const &p;
    bool isrevert;
};

}  // namespace Sealib

#endif  // SRC_FLOW_INOUTGRAPH_H_
