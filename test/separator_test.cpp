#include <sealib/flow/separator.h>
#include <gtest/gtest.h>
#include <sealib/iterator/dfs.h>
#include <cstdio>
#include <cstdlib>
#include <random>
#include <sealib/collection/bitset.h>
#include <sealib/graph/graphcreator.h>

namespace Sealib {

static uint64_t n = 50;
static uint64_t deg = 4;

TEST(SeparatorTest, VSeparator) {
    Bitset<> s = Bitset<>(n);
    Bitset<> t = Bitset<>(n);
    UndirectedGraph g = GraphCreator::kRegular(n, deg);
    for (uint64_t i = 0; i < 5; i++) {
        s.insert(i, true);
    }
    // finding 5 separate vertices from all s vertices
    uint64_t nt = 5;
    for (uint64_t i = 5; i < n; i++) {
        bool align = false;
        for (uint64_t j = 0; j < 5; j++) {
            for (uint64_t k = 0; k < g.deg(j); k++) {
                if (g.head(j, k) == i) align = true;
            }
        }
        if (!align && nt > 0) {
            t.insert(i, true);
            nt--;
        } else {
            t.insert(i, false);
        }
    }
    Bitset<> vs = Bitset<>(n);
    vs = Separator::standardVSeparate(s, t, g, 50, DFS::getStandardDFSIterator);

    // erase connections from other vertices to vertices in vs
    for (uint64_t i = 0; i < n; i++) {
        if (vs.get(i)) {
            for (uint64_t j = 0; j < g.getNode(i).getDegree(); j++) {
                g.getNode(g.getNode(i).getAdj()[j].first)
                    .getAdj()[g.getNode(i).getAdj()[j].second]
                    .first = g.getNode(i).getAdj()[j].first;
                g.getNode(g.getNode(i).getAdj()[j].first)
                    .getAdj()[g.getNode(i).getAdj()[j].second]
                    .second = g.getNode(i).getAdj()[j].second;
            }
        }
    }

    // searching t vertices starting at all vertices in s
    for (uint64_t i = 0; i < n; i++) {
        if (s.get(i)) {
            Iterator<UserCall> *iter = DFS::getStandardDFSIterator(g, i);
            UserCall x = iter->next();
            while (x.type != UserCall::postexplore || x.u != i) {
                switch (x.type) {
                    case UserCall::preexplore:
                        if (t.get(x.u)) FAIL();
                        break;
                }
                x = iter->next();
            }
            free(iter);
        }
    }
    SUCCEED();
}

TEST(SeparatorTest, ESeparator) {
    Bitset<> s = Bitset<>(n);
    Bitset<> t = Bitset<>(n);
    UndirectedGraph g = GraphCreator::kRegular(n, deg);
    for (uint64_t i = 0; i < 5; i++) {
        s.insert(i, true);
    }
    // finding 5 separate vertices from all s vertices
    uint64_t nt = 5;
    for (uint64_t i = 5; i < n; i++) {
        bool align = false;
        for (uint64_t j = 0; j < 5; j++) {
            for (uint64_t k = 0; k < g.deg(j); k++) {
                if (g.head(j, k) == i) align = true;
            }
        }
        if (!align && nt > 0) {
            t.insert(i, true);
            nt--;
        } else {
            t.insert(i, false);
        }
    }
    std::vector<std::pair<uint64_t, uint64_t>> es;
    es = Separator::standardESeparate(s, t, g, 50, DFS::getStandardDFSIterator);

    // erase edges (make u = k)
    for (uint64_t i = 0; i < es.size(); i++) {
        for (uint64_t j = 0; j < g.getNode(es[i].first).getDegree(); j++) {
            if (g.head(es[i].first, j) == es[i].second) {
                g.getNode(es[i].first).getAdj()[j].first = es[i].first;
            }
        }
    }

    // searching t vertices starting at all vertices in s
    for (uint64_t i = 0; i < n; i++) {
        if (s.get(i)) {
            Iterator<UserCall> *iter = DFS::getStandardDFSIterator(g, i);
            UserCall x = iter->next();
            while (x.type != UserCall::postexplore || x.u != i) {
                switch (x.type) {
                    case UserCall::preexplore:
                        if (t.get(x.u)) FAIL();
                        break;
                }
                x = iter->next();
            }
            free(iter);
        }
    }
    SUCCEED();
}

}  // namespace Sealib
