#include <gtest/gtest.h>
#include <sealib/collection/bitset.h>
#include <sealib/flow/separator.h>
#include <sealib/graph/graphcreator.h>
#include <sealib/iterator/dfs.h>
#include <cstdio>
#include <cstdlib>
#include <random>
#include "../src/dfs/standarddfsiterator.h"

namespace Sealib {

static uint64_t n = 300;
static uint64_t k = 50;
static uint64_t s_count = 5;
static uint64_t t_count = 5;
static uint64_t deg = 4;
static Bitset<> s = Bitset<>(n);
static Bitset<> t = Bitset<>(n);
static UndirectedGraph g = GraphCreator::(n, deg);

void initST() {
    for (uint64_t i = 0; i < n; i++) {
        if (i < s_count)
            s.insert(i, true);
        else
            s.insert(i, false);
    }
    // finding 5 separate vertices from all s vertices
    uint64_t nt = t_count;
    for (uint64_t i = s_count; i < n; i++) {
        bool align = false;
        for (uint64_t k = 0; k < g.deg(i); k++) {
            if (s.get(g.head(i, k)) == i) align = true;
        }
        if (!align && nt > 0) {
            t.insert(i, true);
            nt--;
        } else {
            t.insert(i, false);
        }
    }
}

TEST(SeparatorTest, VSeparator) {
    initST();
    Bitset<> vs, vs2;
    try {
        vs = Separator::nBitVSeparate(s, t, g, k, DFS::getStandardDFSIterator);
    } catch (const char *e) {
        printf("%s\n", e);
    }
    try {
        vs2 = Separator::standardVSeparate(s, t, g, k,
                                           DFS::getStandardDFSIterator);
    } catch (const char *e) {
        printf("%s\n", e);
    }
    uint64_t x = 0, y = 0;
    for (uint64_t i = 0; i < n; i++) {
        if (vs.get(i)) x++;
        if (vs2.get(i)) y++;
    }
    // minimum cut can*t be bigger than all neigbours from all vertices in s
    EXPECT_LE(x, s_count * deg);
    // bouth compute a minimum cut, so their size should be equal
    EXPECT_EQ(x, y);
    // searching t vertices starting at all vertices in s
    for (uint64_t i = 0; i < n; i++) {
        if (s.get(i)) {
            Iterator<UserCall> *iter = DFS::getStandardDFSIterator(g, i);
            UserCall x = iter->next();
            while (x.type != UserCall::postprocess || x.u != i) {
                switch (x.type) {
                    case UserCall::preprocess:
                        if (t.get(x.u)) FAIL();
                        break;
                    case UserCall::preexplore:
                        if (vs.get(g.head(x.u, x.k)))
                            ((StandardDFSIterator *)iter)->skip();
                        break;
                }
                x = iter->next();
            }
            free(iter);
        }
    }
    for (uint64_t i = 0; i < n; i++) {
        if (s.get(i)) {
            Iterator<UserCall> *iter = DFS::getStandardDFSIterator(g, i);
            UserCall x = iter->next();
            while (x.type != UserCall::postprocess || x.u != i) {
                switch (x.type) {
                    case UserCall::preprocess:
                        if (t.get(x.u)) FAIL();
                        break;
                    case UserCall::preexplore:
                        if (vs2.get(g.head(x.u, x.k)))
                            ((StandardDFSIterator *)iter)->skip();
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
    initST();
    std::vector<std::pair<uint64_t, uint64_t>> es;
    try {
        es = Separator::standardESeparate(s, t, g, k,
                                          DFS::getStandardDFSIterator);
    } catch (const char *e) {
        printf("%s\n", e);
    }

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
            while (x.type != UserCall::postprocess || x.u != i) {
                switch (x.type) {
                    case UserCall::preprocess:
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
