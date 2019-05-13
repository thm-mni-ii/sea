#include "sealib/graph/virtualgraph.h"

namespace Sealib {

VirtualGraph::VirtualGraph(UndirectedGraph const &graph)
    : UndirectedGraph(0),
      g(graph),
      n(g.getOrder()),
      presentVertices(n),
      virtualEdges(4 * n) {
    presentVertices.flip();
}

uint64_t VirtualGraph::deg(uint64_t u) const { return deg(u, true); }

uint64_t VirtualGraph::deg(uint64_t u, bool artificialEdges) const {
    uint64_t a = 0;
    for (uint64_t b = 0; b < g.deg(u); b++) {
        if (presentVertices[g.head(u, b)]) {
            a++;
        }
    }
    if (artificialEdges && virtualEdges.member(u)) {
        std::pair<uint64_t, uint64_t> p = virtualEdges.get(u);
        if (p.second != INVALID) {
            a += 2;
        } else {
            a++;
        }
    }
    return a;
}

uint64_t VirtualGraph::head(uint64_t u, uint64_t k) const {
    uint64_t a = 0;
    for (uint64_t b = 0; b < g.deg(u); b++) {
        if (presentVertices[g.head(u, b)]) {
            if (a == k) {
                return g.head(u, b);
            }
            a++;
        }
    }
    if (a >= k - 1 && virtualEdges.member(u)) {
        std::pair<uint64_t, uint64_t> p = virtualEdges.get(u);
        if (a == k) {
            return p.first;
        } else if (a == k - 1) {
            return p.second;
        }
    }
    return INVALID;
}

uint64_t VirtualGraph::mate(uint64_t u, uint64_t k) const {
    uint64_t a = 0;
    for (uint64_t b = 0; b < g.deg(u); b++) {
        if (presentVertices[g.head(u, b)]) {
            if (a == k) {
                return g.mate(u, b) - (b - a);
            }
            a++;
        }
    }
    if (a >= k - 1 && virtualEdges.member(u)) {
        std::pair<uint64_t, uint64_t> q = virtualEdges.get(head(u, k));
        assert(q.first == u || q.second == u);
        return q.first == u && q.second != INVALID ? deg(head(u, k)) - 2
                                                   : deg(head(u, k)) - 1;
    }
    return INVALID;
}

uint64_t VirtualGraph::index(uint64_t u, uint64_t k) const {
    uint64_t a = 0;
    for (uint64_t b = 0; b < g.deg(u); b++) {
        if (presentVertices[g.head(u, b)]) {
            if (a == k) {
                return b;
            }
            a++;
        }
    }
    if (a >= k - 1 && virtualEdges.member(u)) {
        if (a == k) {
            return g.deg(u) - 2;
        } else if (a == k - 1) {
            return g.deg(u) - 1;
        }
    }
    return INVALID;
}

uint64_t VirtualGraph::getOrder() const { return n; }

void VirtualGraph::removeVertex(uint64_t u) {
    presentVertices[u] = 0;
    n--;
}

bool VirtualGraph::hasVertex(uint64_t u) const { return presentVertices[u]; }

void VirtualGraph::addEdge(uint64_t u, uint64_t v) {
    if (virtualEdges.member(u)) {
        // update with second edge
        virtualEdges.insert(u, {virtualEdges.get(u).first, v});
    } else {
        // add first edge
        virtualEdges.insert(u, {v, INVALID});
    }
    if (virtualEdges.member(v)) {
        virtualEdges.insert(v, {virtualEdges.get(v).first, u});
    } else {
        virtualEdges.insert(v, {u, INVALID});
    }
}

void VirtualGraph::removeEdge(uint64_t u, uint64_t v) {
    // delete edge between u and v
    if (virtualEdges.member(u) && virtualEdges.member(v)) {
        // search virtual edge {u,v}
        std::pair<uint64_t, uint64_t> pu = virtualEdges.get(u),
                                      pv = virtualEdges.get(v);
        if (pu.second != INVALID) {
            virtualEdges.insert(
                u, {pu.first == v ? pu.second : pu.first, INVALID});
        } else {
            virtualEdges.remove(u);
        }
        if (pv.second != INVALID) {
            virtualEdges.insert(
                v, {pv.first == u ? pv.second : pv.first, INVALID});
        } else {
            virtualEdges.remove(v);
        }
    }
}

}  // namespace Sealib
