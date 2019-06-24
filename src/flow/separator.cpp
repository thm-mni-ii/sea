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
        k--;
        ispath = false;
        std::vector<uint64_t> path(0);
        Iterator<UserCall> *it = iter(graph, graph.getOrder() - 1);
        UserCall x = it->next();
        while (
            (x.type != UserCall::postprocess || x.u != graph.getOrder() - 1) &&
            !ispath) {
            switch (x.type) {
                case UserCall::preprocess:
                    path.push_back(x.u);
                    if (x.u < t.size()) {
                        if (t.get(x.u)) ispath = true;
                    }
                    break;
                case UserCall::postprocess:
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
    while (x.type != UserCall::postprocess || x.u != graph.getOrder() - 1) {
        switch (x.type) {
            case UserCall::preprocess:
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

std::vector<std::pair<uint64_t, uint64_t>> Separator::eSeparate(
        Sealib::Bitset<> const &s, Sealib::Bitset<> const &t,
        Sealib::Graph const &g, int64_t k,
        Iterator<UserCall> *iter(Graph const &, uint64_t)) {

    std::vector<std::pair<uint8_t, uint8_t>> paths(g.getOrder());
    // compute a residual network
    bool ispath = true;
    bool hasNodesLeft = false;
    uint64_t si = 0;
    uint8_t path = 1;
    Iterator<UserCall> *it = nullptr;
    while (ispath && k >= 0) {
        k--;
        ispath = false;
        while(si < s.size()){
            if(s[si]) {
                ispath = true;
                break;
            } else {
                si++;
            }
        }
        if(si < s.size()) {
            it = iter(g, si);
            UserCall x = it->next();
            uint8_t order = 0;
            while (
                    x.type != UserCall::postprocess || x.u != si) {
                switch (x.type) {
                    case UserCall::preprocess:
                        paths[x.u].second = 0;
                        if (t.get(x.u)){
                            ispath = true;
                        } else if(paths[x.u].first == 0){
                            paths[x.u].first = path;
                        }
                        break;
                    case UserCall::preexplore:
                        if(paths[x.u].first != 0){
                            uint8_t thisPath = paths[x.u].first;
                            if (thisPath == paths[g.head(x.u, x.k)].first){
                                paths[g.head(x.u, x.k)].second = 1;
                                ((SkipableIterator *)it)->skip();
                            }
                        }
                        break;
                    case UserCall::postexplore:
                        if(ispath && x.u == si){
                            x = it->next();
                            if(!(x.type == UserCall::postprocess && x.u == si)){
                                free(it);
                                it = iter(g, si);
                                ispath = false;
                            }
                        }
                        if(paths[g.head(x.u, x.k)].second == 1){
                            Iterator<UserCall> *iterim = iter(g, x.u);
                            UserCall y = iterim->next();
                            while (
                                    (x.type != UserCall::postprocess || y.u != x.u) &&
                                    !ispath) {
                                bool remove = false;
                                switch (x.type) {
                                    case UserCall::preprocess:
                                        if(paths[y.u].first == path && paths[y.u].second == 2) {
                                            if (remove) paths[y.u].first = 0;
                                            if (y.u == x.u) remove = true;
                                            if (paths[y.u].second == 1) remove = false;
                                        } else {
                                            ((SkipableIterator *)iterim)->skip();
                                        }
                                        break;
                                }
                                y = iterim->next();
                            }
                            free(iterim);
                        }
                        paths[g.head(x.u, x.k)].second = 2;
                        break;
                    case UserCall::postprocess:
                        if(!ispath && paths[x.u].first == path){
                            paths[x.u].first = 0;
                            paths[x.u].second = 0;
                        }
                        break;
                }
                x = it->next();
                order++;
            }
        }
        si++;
    }
    if (k < 0) throw std::exception();  // interrupt

    // compute a set of edges in the minimum cut
    std::vector<std::pair<uint64_t, uint64_t>> es;

    si = 0;
    path = 0;
    while(si < s.size()) {
        while (si < s.size()) {
            if (s[si]) {
                break;
            } else {
                si++;
            }
        }
        if (si < s.size()) {
            it = iter(g, si);
            UserCall x = it->next();
            while (x.type != UserCall::postprocess || x.u != g.getOrder() - 1) {
                switch (x.type) {
                    case UserCall::preprocess:
                        paths[x.u].second = paths[x.u].second | 0x10;
                        break;
                    case UserCall::preexplore:
                        if(paths[x.u].first == paths[g.head(x.u, x.k)].first){
                            if(!(paths[x.u].second == (paths[g.head(x.u, x.k)].second+1)%3)){
                                ((SkipableIterator *)it)->skip();
                            }
                        }
                        break;
                }
                x = it->next();
            }
            free(it);
        }
        si++;
    }
    for (uint64_t i = 0; i < g.getOrder(); i++) {
        for (uint64_t j = 0; j < g.deg(i); j++) {
            if ((paths[i].second & 0x10) == !(paths[g.head(i, j)].second & 0x10)) {
                es.emplace_back(i, g.head(i, j));
            }
        }
    }
    return es;
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

Sealib::Bitset<> Separator::vSeparate(
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
        es =
                Separator::eSeparate(s2, t2, InOutGraph(g), k, iter);
    } catch (std::exception &e) {
        throw e;
    }

    // get the minimum vertex separator
    Sealib::Bitset<> vs = Sealib::Bitset<uint64_t>(g.getOrder());
    for (uint64_t i = 0; i < es.size(); i++) {
        if (!(s2[es[i].first] || s2[es[i].second])) {
            if (es[i].first < vs.size()) vs[es[i].first] = true;
            else vs[es[i].second] = true;
        } else {
            if (es[i].first < vs.size()) vs[es[i].second - g.getOrder()] = true;
            else vs[es[i].first - g.getOrder()] = true;
        }
    }
    return vs;
}

}  // namespace Sealib
