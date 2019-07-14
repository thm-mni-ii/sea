#include <sealib/flow/separator.h>
#include <sealib/graph/directedgraph.h>
#include <sealib/graph/graphcreator.h>
#include <sealib/graph/node.h>
#include <sealib/iterator/dfs.h>
#include "inoutgraph.h"
#include "stgraph.h"
#include "../dfs/skipableiterator.h"

namespace Sealib {

std::vector<std::pair<uint64_t, uint64_t>> Separator::standardESeparate(
    Sealib::Bitset<> const &s, Sealib::Bitset<> const &t,
    Sealib::Graph const &g, int64_t k,
    Iterator<UserCall> *iter(Graph const &, uint64_t)) {
    STGraph graph = Sealib::STGraph(g, s, t);

    // compute a residual network
    bool ispath = true;
    while (ispath && k >= 0) {
        ispath = false;
        std::vector<uint64_t> path;
        Iterator<UserCall> *it = iter(graph, graph.getOrder() - 1);
        UserCall x = it->next();
        while (
            (x.type != UserCall::postprocess || x.u != graph.getOrder() - 1) &&
            !ispath) {
            switch (x.type) {
                case UserCall::preexplore:
                    if (!ispath) {
                        path.push_back(x.k);
                        if (graph.head(x.u, x.k) < t.size()) {
                            if (t.get(graph.head(x.u, x.k))) ispath = true;
                        }
                    } else {
                        ((SkipableIterator *)it)->skip();
                    }
                    break;
                case UserCall::postexplore:
                    if (!ispath) path.pop_back();
                    break;
            }
            x = it->next();
        }
        free(it);
        if (ispath) {
            graph.revertpath(path);
            k--;
        }
    }
    if (k < 0) throw "There is no edge seperator in k range.";  // interrupt

    // compute a set of edges in the minimum cut
    Sealib::Bitset<> s_reachable = Sealib::Bitset<uint64_t>(graph.getOrder());
    Iterator<UserCall> *it = iter(graph, graph.getOrder() - 1);
    UserCall x = it->next();
    while (x.type != UserCall::postprocess || x.u != graph.getOrder() - 1) {
        switch (x.type) {
            case UserCall::preprocess:
                s_reachable[x.u] = true;
                break;
        }
        x = it->next();
    }
    for (uint64_t i = 0; i < s_reachable.size(); i++) {
        printf("%d:\t%d\n", i, s_reachable.get(i));
    }
    free(it);
    std::vector<std::pair<uint64_t, uint64_t>> es;
    for (uint64_t i = 0; i < g.getOrder(); i++) {
        for (uint64_t j = 0; j < g.deg(i); j++) {
            if (s_reachable.get(i) != s_reachable.get(g.head(i, j))) {
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
    } catch (const char *e) {
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
        nodes.emplace_back(x);
    }
    for (uint64_t i = 0; i < g.getOrder(); i++) {
        SimpleNode x;
        for (uint64_t j = 0; j < g.deg(i); j++) {
            x.addAdjacency(g.head(i, j));
        }
        nodes.emplace_back(x);
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
    } catch (const char *e) {
        throw "There is no edge seperator in k range.";
    }

    // get the minimum vertex separator
    Sealib::Bitset<> vs = Sealib::Bitset<uint64_t>(g.getOrder());
    for (uint64_t i = 0; i < es.size(); i++) {
        if (!(s2[es[i].first] || s2[es[i].second])) {
            if (es[i].first < g.getOrder()) {
                vs[es[i].first] = true;
            } else {
                vs[es[i].second] = true;
            }
        } else if (s2[es[i].first]) {
            if (es[i].second < g.getOrder()) {
                vs[es[i].second] = true;
            } else {
                vs[es[i].second - g.getOrder()] = true;
            }
        } else if (s2[es[i].second]) {
            if (es[i].first < g.getOrder()) {
                vs[es[i].first] = true;
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
    } catch (const char *e) {
        throw e;
    }
}

Sealib::Bitset<> Separator::nBitVSeparate(
    Sealib::Bitset<> const &s, Sealib::Bitset<> const &t,
    Sealib::Graph const &g, int64_t k,
    Iterator<UserCall> *iter(Graph const &, uint64_t)) {
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
        es = Separator::inOutGraphESeparate(s2, t2, g, k, iter);
    } catch (const char *e) {
        throw e;
    }

    // get the minimum vertex separator
    Sealib::Bitset<> vs = Sealib::Bitset<uint64_t>(g.getOrder());
    for (uint64_t i = 0; i < es.size(); i++) {
        if (!(s2[es[i].first] || s2[es[i].second])) {
            if (es[i].first < g.getOrder()) {
                vs[es[i].first] = true;
            } else {
                vs[es[i].second] = true;
            }
        } else if (s2[es[i].first]) {
            if (es[i].second < g.getOrder()) {
                vs[es[i].second] = true;
            } else {
                vs[es[i].second - g.getOrder()] = true;
            }
        } else if (s2[es[i].second]) {
            if (es[i].first < g.getOrder()) {
                vs[es[i].first] = true;
            } else {
                vs[es[i].first - g.getOrder()] = true;
            }
        }
    }
    return vs;
}

std::vector<std::pair<uint64_t, uint64_t>> Separator::inOutGraphESeparate(
    Sealib::Bitset<> const &s, Sealib::Bitset<> const &t,
    Sealib::Graph const &graph, int64_t k,
    Iterator<UserCall> *iter(Graph const &, uint64_t)) {
    std::vector<std::pair<bool, std::pair<uint8_t, uint8_t>>> paths(
        2 * graph.getOrder());
    Bitset<uint8_t> needFix((uint64_t)k);
    InOutGraph g = InOutGraph(graph, paths);
    g.setisrevert(true);
    // compute a residual network
    std::vector<uint64_t> s_nodes;
    for (uint64_t i = 0; i < graph.getOrder(); i++) {
        if (s[i]) s_nodes.emplace_back(i + graph.getOrder());
    }
    uint8_t path = 1;
    Iterator<UserCall> *it = nullptr;

    for (uint64_t i = 0; i < s_nodes.size(); i++) {
        bool isPath = true;
        while (isPath) {
            isPath = false;
            it = iter(g, s_nodes[i]);
            UserCall x = it->next();
            needFix.insert(path, true);
            while (x.type != UserCall::postprocess || x.u != s_nodes[i]) {
                switch (x.type) {
                    case UserCall::preprocess:
                        if (isPath) {
                            ((SkipableIterator *)it)->skip();
                        } else {
                            if (t[x.u])
                                isPath = true;
                            else if (paths[x.u].second.first == 0) {
                                paths[x.u].second.first = path;
                            }
                        }
                        break;
                    case UserCall::preexplore:
                        if (isPath) {
                            ((SkipableIterator *)it)->skip();
                        } else if (paths[g.head(x.u, x.k)].second.first != 0) {
                            if (paths[g.head(x.u, x.k)].second.first ==
                                paths[x.u].second.second) {
                                paths[g.head(x.u, x.k)].second.second = path;
                            } else if (paths[g.head(x.u, x.k)].second.first ==
                                       paths[x.u].second.first) {
                                paths[g.head(x.u, x.k)].second.second = path;
                                paths[x.u].second.first = 0;
                                paths[x.u].second.second = 0;
                            } else {
                                needFix.insert(
                                    paths[g.head(x.u, x.k)].second.first, true);
                                paths[g.head(x.u, x.k)].second.second =
                                    paths[g.head(x.u, x.k)].second.first;
                                paths[g.head(x.u, x.k)].second.first = path;
                            }
                        }
                        break;
                    case UserCall::postexplore:
                        if (paths[g.head(x.u, x.k)].second.second != 0 &&
                            !isPath) {
                            if (paths[x.u].second.second ==
                                paths[g.head(x.u, x.k)].second.first) {
                                paths[x.u].second.second = 0;
                                paths[g.head(x.u, x.k)].second.second = 0;
                                paths[x.u].second.first =
                                    paths[g.head(x.u, x.k)].second.first;
                            } else {
                                paths[g.head(x.u, x.k)].second.second = 0;
                                paths[x.u].second.first =
                                    paths[g.head(x.u, x.k)].second.first;
                                if (paths[x.u].second.second != 0)
                                    paths[x.u].second.second = path;
                            }
                        }
                        break;
                    case UserCall::postprocess:
                        if (paths[x.u].second.first == path && !isPath) {
                            paths[x.u].second.first = 0;
                        }
                        break;
                }
                x = it->next();
            }
            if (isPath) {
                g.setisrevert(false);
                for (uint64_t i = 0; i < g.getOrder(); i++) {
                    if (s[i]) {
                        for (uint64_t j = 0; j < g.deg(i); j++) {
                            if (needFix.get(paths[g.head(i, j)].second.first)) {
                                fixPath(g.head(i, j), g, paths, iter);
                            }
                        }
                    }
                }
                needFix.clear();
                k--;
            }
        }
    }
    if (k < 0) throw "There is no edge seperator in k range.";  // interrupt

    // compute a set of edges in the minimum cut
    g.setisrevert(true);
    for (uint64_t i = 0; i < s_nodes.size(); i++) {
        it = iter(g, s_nodes[i]);
        UserCall x = it->next();
        while (x.type != UserCall::postprocess || x.u != s_nodes[i]) {
            switch (x.type) {
                case UserCall::preprocess:
                    paths[x.u].first = true;
                    break;
            }
            x = it->next();
        }
        free(it);
    }
    for (uint64_t i = 0; i < paths.size(); i++) {
        printf("%d:\t%d\n", i, paths[i].first);
    }
    std::vector<std::pair<uint64_t, uint64_t>> es;
    for (uint64_t i = 0; i < graph.getOrder(); i++) {
        for (uint64_t j = 0; j < graph.deg(i); j++) {
            if (paths[i].first != paths[graph.head(i, j)].first) {
                es.emplace_back(i, graph.head(i, j));
            }
        }
    }
    return es;
}

void Separator::fixPath(
    uint64_t start, InOutGraph g,
    std::vector<std::pair<bool, std::pair<uint8_t, uint8_t>>> &paths,
    Iterator<UserCall> *iter(Graph const &, uint64_t)) {
    uint8_t path = paths[start].second.first;
    uint8_t temp = path;
    uint64_t neightbours = 0;
    Iterator<UserCall> *it = nullptr;
    it = iter(g, start);
    UserCall x = it->next();
    while (x.type != UserCall::postprocess || x.u != start) {
        switch (x.type) {
            case UserCall::preprocess:
                if (paths[x.u].second.first == temp) {
                    if (paths[x.u].second.second != 0) {
                        temp = paths[x.u].second.second;
                        paths[x.u].second.second = 0;
                    }
                    paths[x.u].second.first = path;
                } else {
                    ((SkipableIterator *)it)->skip();
                }
                break;
            case UserCall::postprocess:
                if (paths[x.u].second.first == path) {
                    if (neightbours > 1) {
                        if (paths[x.u].first) {
                            neightbours--;
                            paths[x.u].first = false;
                        }
                        paths[x.u].first = 0;
                    } else {
                        g.setisrevert(true);
                        for (uint64_t i = 0; i < g.deg(x.u); i++) {
                            if (paths[g.head(x.u, i)].second.first == path) {
                                paths[g.head(x.u, i)].first = true;
                                neightbours++;
                            }
                        }
                    }
                }
                break;
        }
        x = it->next();
    }
    free(it);
}

}  // namespace Sealib
