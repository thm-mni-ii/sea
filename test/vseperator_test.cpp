#include "sealib/graph/graphcreator.h"
#include "sealib/collection/bitset.h"
#include <gtest/gtest.h>
#include <cstdio>
#include <cstdlib>
#include <random>
#include "sealib/flow/vseperator.h"

namespace Sealib{

static uint64_t n = 200;
static std::random_device rnd;

TEST(VSeperatorTest, adjMatrix) {
    std::uniform_int_distribution<uint64_t> edgeR(0, 2);
    Bitset<> s = Bitset<>(n);
    Bitset<> t = Bitset<>(n);
    uint64_t **m =
            static_cast<uint64_t **>(std::malloc(n * sizeof(uint64_t *)));
    for (uint64_t u = 0; u < n; u++) {
        m[u] = static_cast<uint64_t *>(std::malloc(n * sizeof(uint64_t)));
    }
    for (uint64_t u = 0; u < n; u++) {
        for (uint64_t v = 0; v < n; v++) {
            uint64_t edges = edgeR(rnd);
            m[u][v] = edges;
            m[v][u] = edges;
        }
    }
    for (uint64_t i = 0; i < 5; i++) {
        s.insert(i, true);
    }
    uint64_t nt = 5;
    for (uint64_t i = 5; i < n; i++) {
        bool align = false;
        for (uint64_t j = 0; j < 5; j++) {
            if (m[j][i] == 1) align = true;
        }
        if (!align && nt > 0) {
            t.insert(i, true);
            nt--;
        } else t.insert(i, false);

    }
    UndirectedGraph g = GraphCreator::createFromAdjacencyMatrix(m, n);
    Bitset<> vs = Bitset<>(n);
    for (uint64_t i = 0; i < n; i++) {
        printf("%d\t%d\t%d\n", s.get(i), t.get(i), vs.get(i));
    }
    vs = VSeperator::seperate(s, t, g, 7);
    FAIL();
}

} // using sealib namespace