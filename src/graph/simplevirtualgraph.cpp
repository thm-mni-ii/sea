#include "./simplevirtualgraph.h"

namespace Sealib {

SimpleVirtualGraph::SimpleVirtualGraph(UndirectedGraph const& g)
    : g(g), n(g.getOrder()), present(n), virtualAdj(n) {
    present.flip();
}

uint64_t SimpleVirtualGraph::head(uint64_t u, uint64_t k) const {
    uint64_t i = 0;
    std::vector<std::pair<uint64_t, uint64_t>> const& adj =
        g.getNode(u).getAdj();
    for (uint64_t a = 0; a < g.deg(u); a++) {
        if (present[adj[a].first]) {
            if (i == k) {
                return adj[a].first;
            }
            i++;
        }
    }
    for (uint64_t a = 0; a < virtualAdj[u].size(); a++) {
        if (present[virtualAdj[u][a]]) {
            if (i == k) {
                return virtualAdj[u][a];
            }
            i++;
        }
    }
    return INVALID;
}

uint64_t SimpleVirtualGraph::deg(uint64_t u) const {
    uint64_t r = 0;
    std::vector<std::pair<uint64_t, uint64_t>> const& adj =
        g.getNode(u).getAdj();
    for (uint64_t a = 0; a < g.deg(u); a++) {
        if (present[adj[a].first]) r++;
    }
    for (uint64_t a = 0; a < virtualAdj[u].size(); a++) {
        if (present[virtualAdj[u][a]]) r++;
    }
    return r;
}

uint64_t SimpleVirtualGraph::getOrder() const { return n; }

uint64_t SimpleVirtualGraph::index(uint64_t u, uint64_t v) const {
    uint64_t b;
    for (b = 0; b < g.deg(u); b++) {
        if (g.head(u, b) == v) return b;
    }
    for (uint64_t a = 0; a < virtualAdj[u].size(); a++) {
        if (virtualAdj[u][a] == v) return b + a;
    }
    return INVALID;
}

void SimpleVirtualGraph::addEdge(uint64_t u, uint64_t v) {
    virtualAdj[u].push_back(v);
    virtualAdj[v].push_back(u);
}

void SimpleVirtualGraph::removeVertex(uint64_t u) {
    present[u] = 0;
    n--;
}

}  // namespace Sealib
