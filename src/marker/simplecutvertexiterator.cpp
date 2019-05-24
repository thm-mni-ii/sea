#include "./simplecutvertexiterator.h"

namespace Sealib {

SimpleCutVertexIterator::SimpleCutVertexIterator(UndirectedGraph const& graph)
    : g(graph), n(g.getOrder()), number(n, INVALID), lowpt(n), parent(n) {}

void SimpleCutVertexIterator::init() {
    for (uint64_t a = 0; a < n; a++) {
        if (number[a] == INVALID) {
            findLowpt(a);
        }
    }
}

void SimpleCutVertexIterator::findLowpt(uint64_t u0) {
    uint64_t rootChildren = 0;
    s.push({u0, 0});
    parent[u0] = INVALID;
    while (!s.empty()) {
        std::pair<uint64_t, uint64_t> p = s.top();
        s.pop();
        uint64_t u = p.first, k = p.second;
        if (number[u] == INVALID) {
            number[u] = lowpt[u] = i;
            i++;
        }
        if (k < g.deg(u)) {
            s.push({u, k + 1});
            if (s.size() > smax) smax = s.size();
            uint64_t v = g.head(u, k);
            if (number[v] == INVALID) {
                s.push({v, 0});
                if (s.size() > smax) smax = s.size();
                parent[v] = u;
                edges.push({v, u});
            } else if (number[v] < number[u] && parent[u] != v) {
                // back edge u->v
                edges.push({u, v});
                if (number[v] < lowpt[u]) {
                    lowpt[u] = number[v];
                }
            }
        } else {
            // we are back to u after computing lowpts of neighbors
            bool isCutvertex = false;
            for (uint64_t l = 0; l < g.deg(u); l++) {
                uint64_t v = g.head(u, l);
                if (parent[v] == u) {
                    // tree edge u->v
                    if (lowpt[v] < lowpt[u]) {
                        lowpt[u] = lowpt[v];
                    }
                    if (u != u0 && lowpt[v] >= number[u]) {
                        isCutvertex = true;
                    } else if (u == u0) {
                        rootChildren++;
                        if (rootChildren > 1) {
                            isCutvertex = true;
                        }
                    }
                }
            }
            if (isCutvertex) cut.push_back(u);
        }
    }
}

bool SimpleCutVertexIterator::more() { return !cut.empty(); }

uint64_t SimpleCutVertexIterator::next() {
    uint64_t r = cut.front();
    cut.pop_front();
    return r;
}

uint64_t SimpleCutVertexIterator::next(BiConsumer onEdge) {
    uint64_t r = cut.front();
    while (number[edges.top().first] <= r) {
        onEdge(edges.top().first, edges.top().second);
        edges.pop();
    }
    // edge {u,v}
    onEdge(edges.top().first, edges.top().second);
    edges.pop();
    cut.pop_front();
    return r;
}

}  // namespace Sealib
