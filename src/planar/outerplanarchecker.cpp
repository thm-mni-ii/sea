#include "sealib/iterator/outerplanarchecker.h"
#include <cmath>
#include "sealib/iterator/choicedictionaryiterator.h"

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

static std::vector<bool> makeBits(UndirectedGraph const &g) {
    std::vector<bool> r;
    for (uint64_t u = 0; u < g.getOrder(); u++) {
        r.push_back(1);
        for (uint64_t k = 0; k < g.deg(u) + 2; k++) {
            r.push_back(0);
        }
    }
    return r;
}

OuterplanarChecker::OuterplanarChecker(UndirectedGraph const &graph)
    : g(graph),
      n(g.getOrder()),
      m(countEdges(graph)),
      d(n),
      di(d),
      tried(n),
      paths(2 * m + 2 * n, 3),
      pathOffset(Bitset<uint8_t>(makeBits(graph))) {}

bool OuterplanarChecker::isOuterplanar() {
    if (m > 2 * n - 3) {
        return false;
    }
    if (!removeClosedChains()) {
        return false;
    }
    if (!removeAllChains()) {
        return false;
    }
    return true;
}

static int const MIN_VERTICES = 3;

bool OuterplanarChecker::removeClosedChains() {
    for (uint64_t round = 0; round < log2(log2(n)) + 1; round++) {
        if (g.getOrder() <= MIN_VERTICES) {
            return true;
        }
        for (uint64_t u = 0; u < n; u++) {
            if (g.hasVertex(u) && g.deg(u, false) == 2) {
                d.insert(u);
            }
        }
        di.init();
        if (!di.more()) {
            return false;
        }
        while (di.more()) {
            if (g.getOrder() <= MIN_VERTICES) {
                return true;
            }
            uint64_t u = di.next();
            ChainData c = chain(u, false);
            if (c.isClosed) {
                bool repeatedOnce = false, repeat;
                do {
                    bool pathStatus = removeChain(c, false);
                    if (!pathStatus) {
                        return false;
                    }
                    if (g.getOrder() <= 3) {
                        return true;
                    }
                    if (c.isCycle) {
                        break;
                    }
                    // if an endpoint becomes degree 2, check resulting chain:
                    // either repeat immediately or end loop
                    repeat = false;
                    for (uint64_t e : {c.c1.first, c.c2.first}) {
                        if (g.deg(e, false) == 2) {
                            ChainData a = chain(e, false);
                            if (a.isTried || (!a.isClosed && repeatedOnce)) {
                                // chain could not be reduced further
                                forEach(a, [this](uint64_t u) {
                                    if (d.get(u)) d.remove(u);
                                });
                                if (!a.isClosed && repeatedOnce) {
                                    tried[a.c1.first] = 1;
                                    tried[a.c2.first] = 1;
                                }
                                break;
                            } else if (a.isClosed) {
                                c = a;
                                repeatedOnce = true;
                                repeat = true;
                            }
                        }
                    }
                } while (repeat);
            } else {
                forEach(c, [this](uint64_t u) { d.remove(u); });
            }
            di.init();  // re-init
        }
        tried.assign(tried.size(), 0);
    }
    return true;
}

bool OuterplanarChecker::removeAllChains() {
    if (g.getOrder() <= MIN_VERTICES) {
        return true;
    }
    // insert one vertex for each good closed chain
    tried.assign(tried.size(), 0);
    for (uint64_t u = 0; u < n; u++) {
        if (g.hasVertex(u) && g.deg(u) == 2 && !tried[u]) {
            ChainData c = chain(u);
            forEach(c, [this](uint64_t u) { tried[u] = 1; });
            if (c.isClosed && c.isGood &&
                !d.get(g.head(c.c1.first, c.c1.second))) {
                d.insert(g.head(c.c1.first, c.c1.second));
            }
        }
    }
    di.init();
    if (!di.more()) {
        return false;
    }
    tried.assign(tried.size(), 0);
    uint64_t next = INVALID;
    while (di.more()) {
        if (g.getOrder() <= MIN_VERTICES) {
            return true;
        }
        uint64_t u;
        if (next == INVALID) {
            u = di.next();
            d.remove(u);
        } else {
            u = next;
            next = INVALID;
        }
        ChainData c = chain(u);
        if (c.isGood) {
            if (!c.isClosed) {
                g.addEdge(c.c1.first, c.c2.first);
            }
            bool pathStatus = removeChain(c);
            if (!pathStatus) {
                return false;
            }
            if (c.isClosed) {
                // check if an endpoint got degree 2, 3 or 4
                for (uint64_t e : {c.c1.first, c.c2.first}) {
                    if (g.deg(e) == 2) {
                        // force evaluation of resulting chain
                        next = e;
                    } else if (g.deg(e) == 3 || g.deg(e) == 4) {
                        for (uint64_t k = 0; k < g.deg(e); k++) {
                            if (g.deg(g.head(e, k)) == 2) {
                                if (tried[g.head(e, k)]) {
                                    assert(next == INVALID &&
                                           "Cannot prioritize two chains at "
                                           "the same time");
                                    next = g.head(e, k);
                                    // we need not set tried[]=false because
                                    // the vertices are deleted anyway in
                                    // the next iteration
                                } else {
                                    d.insert(g.head(e, k));
                                }
                            }
                        }
                    }
                }
            } else {
                g.addEdge(c.c1.first, c.c2.first);
            }
        } else {
            // remove obsolete vertices from D
            forEach(c, [this](uint64_t u) {
                if (d.get(u)) {
                    d.remove(u);
                }
            });
            for (std::pair<uint64_t, uint64_t> e : {c.c1, c.c2}) {
                tried[g.head(e.first, e.second)] = true;
            }
        }
        di.init();
    }
    return true;
}

OuterplanarChecker::ChainData OuterplanarChecker::chain(uint64_t u,
                                                        bool phase2) {
    ChainData r;
    assert(g.deg(u, phase2) == 2);
    uint64_t v1 = g.head(u, 0), v2 = g.head(u, 1);
    uint64_t p1 = u, p2 = u;
    while (g.deg(v1, phase2) == 2 || g.deg(v2, phase2) == 2) {
        if (v1 == u || v2 == u) {
            // cycle detected (a cycle is a valid chain as long as the two
            // chosen endpoints are adjacent)
            r.isCycle = true;
            break;
        }
        if (g.deg(v1, phase2) == 2) {
            uint64_t k1 = g.head(v1, 0) == p1;
            p1 = v1;
            if (!phase2 && tried[v1]) {
                r.isTried = true;
            }
            v1 = g.head(v1, k1);
        }
        if (g.deg(v2, phase2) == 2) {
            uint64_t k2 = g.head(v2, 0) == p2;
            p2 = v2;
            if (!phase2 && tried[v2]) {
                r.isTried = true;
            }
            v2 = g.head(v2, k2);
        }
    }
    if (r.isCycle) {
        uint64_t a = g.head(u, 0);
        r.c1 = {u, 1}, r.c2 = {a, g.head(a, 0) == u};
        r.isClosed = true, r.isGood = true;
    } else {
        // find edges pointing inwards
        uint64_t k1 = INVALID, k2 = INVALID;
        for (uint64_t k = 0; k < g.deg(v1, phase2); k++) {
            if (g.head(v1, k) == p1) {
                k1 = k;
                break;
            }
        }
        for (uint64_t k = 0; k < g.deg(v2, phase2); k++) {
            if (g.head(v2, k) == p2) {
                k2 = k;
                break;
            }
        }
        assert(k1 != INVALID && k2 != INVALID);
        r.c1 = {v1, k1}, r.c2 = {v2, k2};
        forEdge(v1, v2, [&r](uint64_t, uint64_t) { r.isClosed = true; });
        r.isGood = g.deg(v1, phase2) <= 4 || g.deg(v2, phase2) <= 4;
    }
    return r;
}

bool OuterplanarChecker::removeChain(ChainData const &c, bool phase2) {
    bool r = true;
    forEach(
        c,
        [this, phase2](uint64_t u) {
            g.removeVertex(u);
            if (!phase2 || d.get(u)) d.remove(u);
        },
        [this, &r](uint64_t u, uint64_t k) {
            if (!incrementPaths(u, k)) {
                r = false;
            }
        });
    if (!c.isCycle) {
        // increment path counter between endpoints
        forEdge(c.c1.first, c.c2.first, [this, &r](uint64_t u, uint64_t k) {
            if (!incrementPaths(u, k)) {
                r = false;
            }
        });
    }
    return r;
}

bool OuterplanarChecker::incrementPaths(uint64_t u, uint64_t k) {
    uint64_t a = (pathOffset.select(u + 1) - u - 1) + g.index(u, k);
    paths.insert(a, paths.get(a) + 1);
    return paths.get(a) <= 2;
}

void OuterplanarChecker::forEdge(uint64_t v1, uint64_t v2,
                                 std::function<void(uint64_t, uint64_t)> f) {
    for (uint64_t k = 0; k < g.deg(v1); k++) {
        if (g.head(v1, k) == v2) {
            f(v1, k);
            break;
        }
    }
    for (uint64_t k = 0; k < g.deg(v2); k++) {
        if (g.head(v2, k) == v1) {
            f(v2, k);
            break;
        }
    }
}

void OuterplanarChecker::forEach(OuterplanarChecker::ChainData const &c,
                                 Consumer v, BiConsumer e) {
    uint64_t u = g.head(c.c1.first, c.c1.second),
             k = g.head(u, 0) == c.c1.first;
    e(c.c1.first, c.c1.second);
    e(u, g.head(u, 0) != c.c1.first);
    while (u != c.c2.first) {
        uint64_t u2 = g.head(u, k);
        e(u, k);                    // forward edge
        e(u2, g.head(u2, 0) != u);  // backward edge
        v(u);
        uint64_t k2 = g.hasVertex(u) ? g.head(u2, 0) == u : 0;
        u = u2;
        k = k2;
    }
}

}  // namespace Sealib
