#include <sealib/flow/separator.h>
#include <sealib/graph/directedgraph.h>
#include <sealib/graph/graphcreator.h>
#include <sealib/graph/node.h>
#include <sealib/iterator/dfs.h>
#include "inoutgraph.h"
#include "stgraph.h"

namespace Sealib {

std::vector<std::pair<uint64_t, uint64_t>> Separator::standardESeparate(
    Sealib::Bitset<> const &s, Sealib::Bitset<> const &t,
    Sealib::Graph const &g, int64_t k,
    Iterator<UserCall> *iter(Graph const &, uint64_t)) {
    STGraph graph = Sealib::STGraph(g, s, t);

    // compute a residual network
    bool ispath = true;
    while (ispath && k >= 0) {
        k--;
        ispath = false;
        std::vector<uint64_t> path(0);
        Iterator<UserCall> *it = iter(graph, graph.getOrder() - 1);
        UserCall x = it->next();
        while (
            (x.type != UserCall::postexplore || x.u != graph.getOrder() - 1) &&
            !ispath) {
            switch (x.type) {
                case UserCall::preexplore:
                    path.push_back(x.u);
                    if (x.u > t.size()) {
                        if (t.get(x.u)) ispath = true;
                    }
                    break;
                case UserCall::postexplore:
                    path.pop_back();
                    break;
            }
            x = it->next();
        }
        free(it);
        if (ispath) graph.revertpath(path);
    }
    if (k < 0) throw std::exception();  // interrupt

    // compute a set of edges in the minimum cut
    std::vector<std::pair<uint64_t, uint64_t>> es;
    Sealib::Bitset<> s_reachable = Sealib::Bitset<uint64_t>(graph.getOrder());
    Iterator<UserCall> *it = iter(graph, graph.getOrder() - 1);
    UserCall x = it->next();
    while (x.type != UserCall::postexplore || x.u != graph.getOrder() - 1) {
        switch (x.type) {
            case UserCall::preexplore:
                s_reachable[x.u] = true;
                break;
        }
        x = it->next();
    }
    free(it);
    for (uint64_t i = 0; i < g.getOrder(); i++) {
        for (uint64_t j = 0; j < g.deg(i); j++) {
            if (s_reachable.get(i) == !s_reachable.get(g.head(i, j))) {
                es.emplace_back(i, g.head(i, j));
            }
        }
    }
    return es;
}

std::vector<std::pair<uint64_t, uint64_t>> Separator::standardESeparate(
    Sealib::Bitset<> const &s, Sealib::Bitset<> const &t,
    Sealib::Graph const &g, int64_t k) {
    try {
        return Separator::standardESeparate(s, t, g, k,
                                            DFS::getStandardDFSIterator);
    } catch (std::exception &e) {
        throw e;
    }
}

Sealib::Bitset<> Separator::standardVSeparate(
    Sealib::Bitset<> const &s, Sealib::Bitset<> const &t,
    Sealib::Graph const &g, int64_t k,
    Iterator<UserCall> *iter(Graph const &, uint64_t)) {
    std::vector<SimpleNode> nodes;
    for (uint64_t i = 0; i < g.getOrder(); i++) {
        SimpleNode x;
        x.addAdjacency(i + g.getOrder());
        nodes.push_back(x);
    }
    for (uint64_t i = 0; i < g.getOrder(); i++) {
        SimpleNode x;
        for (uint64_t j = 0; j < g.deg(i); j++) {
            x.addAdjacency(g.head(i, j));
        }
        nodes.push_back(x);
    }

    std::vector<std::pair<uint64_t, uint64_t>> es;
    Sealib::Bitset<> s2 = Sealib::Bitset<uint64_t>(s.size() * 2);
    Sealib::Bitset<> t2 = Sealib::Bitset<uint64_t>(t.size() * 2);
    for (uint64_t i = 0; i < s.size(); i++) {
        s2[i] = s[i];
        s2[s.size() + i] = s[i];
        t2[i] = t[i];
        t2[t.size() + i] = t[i];
    }
    try {
        es =
            Separator::standardESeparate(s2, t2, DirectedGraph(nodes), k, iter);
    } catch (std::exception &e) {
        throw e;
    }

    // get the minimum vertex separator
    Sealib::Bitset<> vs = Sealib::Bitset<uint64_t>(g.getOrder());
    for (uint64_t i = 0; i < es.size(); i++) {
        if (!(s2[es[i].first] || s2[es[i].second])) {
            if (es[i].first < vs.size()) {
                vs[es[i].first] = true;
            } else {
                vs[es[i].second] = true;
            }
        } else {
            if (es[i].first < vs.size()) {
                vs[es[i].second - g.getOrder()] = true;
            } else {
                vs[es[i].first - g.getOrder()] = true;
            }
        }
    }
    return vs;
}

Sealib::Bitset<> Separator::standardVSeparate(Sealib::Bitset<> const &s,
                                              Sealib::Bitset<> const &t,
                                              Sealib::Graph const &g,
                                              int64_t k) {
    try {
        return Separator::standardVSeparate(s, t, g, k,
                                            DFS::getStandardDFSIterator);
    } catch (std::exception &e) {
        throw e;
    }
}

}  // namespace Sealib
