#include "./simpleouterplanarchecker.h"

namespace Sealib {

static uint64_t countEdges(UndirectedGraph const &g) {
    uint64_t r = 0;
    for (uint64_t a = 0; a < g.getOrder(); a++) {
        for (uint64_t b = 0; b < g.deg(a); b++) {
            r++;
        }
    }
    return r / 2;
}

SimpleOuterplanarChecker::SimpleOuterplanarChecker(UndirectedGraph const &graph)
    : g(graph), n(graph.getOrder()), m(countEdges(graph)), isOuter(n) {
    for (uint64_t u = 0; u < n; u++) {
        for (uint64_t k = 0; k < g.deg(u); k++) {
            isOuter[u].push_back(0);
        }
    }
}

bool SimpleOuterplanarChecker::isOuterplanar() {
    if (m > 2 * n - 3) {
        return false;
    }

    for (uint64_t u = 0; u < g.getOrder(); u++) {
        if (g.deg(u) < 2) {
            return false;
        } else if (g.deg(u) == 2) {
            degTwo.push(u);
        }
    }
    if (degTwo.size() > dmax) dmax = degTwo.size();

    while (g.getOrder() > 3) {
        if (degTwo.empty()) {
            return false;
        }
        uint64_t u = degTwo.front();
        degTwo.pop();
        g.removeVertex(u);
        uint64_t v0 = g.head(u, 0), v1 = g.head(u, 1);
        if (edgeBetween(v0, v1)) {
            if (g.deg(v0) == 2) degTwo.push(v0);
            if (g.deg(v1) == 2) degTwo.push(v1);
            uint64_t i0 = g.index(v0, v1), i1 = g.index(v1, v0);
            if (!isOuter[v0][i0]) {
                isOuter[v0][i0] = isOuter[v1][i1] = 1;
            } else {
                return false;
            }
        } else {
            g.addEdge(v0, v1);
            isOuter[v0].push_back(1);
            isOuter[v1].push_back(1);
        }
    }
    return true;
}

bool SimpleOuterplanarChecker::edgeBetween(uint64_t u, uint64_t v) {
    for (uint64_t a = 0; a < g.deg(u); a++) {
        if (g.head(u, a) == v) return true;
    }
    return false;
}

}  // namespace Sealib
