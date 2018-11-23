#include "sealib/graph/graphrepresentations.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <random>
#include <set>
#include <vector>
#include "sealib/graph/adjacency.h"
#include "sealib/graph/compactgraph.h"
#include "sealib/graph/node.h"

using Sealib::CompactGraph;
using Sealib::Graphrepresentations;

void Graphrepresentations::standardToCrosspointer(uint32_t* a) {
    uint32_t n = a[0], v, u, pv, pu;
    // n = order of the graph
    v = pv = pu = 0;
    u = 1;
    while (u < n) {
        while (!(a[a[u]] > n || a[u] == a[u + 1])) {
            pu = a[u];
            v = a[pu];
            pv = a[v];
            a[pu] = pv;
            a[pv] = pu;
            ++a[v];
            ++a[u];
        }
        u = u + 1;
    }
    // restore T pointers
    v = n;
    while (v > 1) {
        a[v] = a[v - 1];
        --v;
    }
    a[1] = n + 2;
    return;
}

void Graphrepresentations::standardToBeginpointer(uint32_t* a) {
    uint32_t order = a[0];
    uint32_t numEdges = a[order + 1];
    uint32_t graphSize = order + numEdges + 2;
    for (uint32_t i = order + 2; i < graphSize; ++i) {
        // checks if a[i] is not a node of order 0
        if (a[a[i]] != a[a[i] - 1] || i == order + 2) {
            a[i] = a[a[i]];
        }
    }
    return;
}

void Graphrepresentations::swappedBeginpointerToStandard(uint32_t* a) {
    uint32_t order = a[0];
    uint32_t numEdges = a[order + 1];
    uint32_t graphSize = order + numEdges + 2;
    for (uint32_t i = order + 2; i < graphSize; ++i) {
        if (a[i] > order) {
            a[i] = a[a[i]];
        }
    }
    for (uint32_t i = 1; i <= order; ++i) {
        a[i] = a[a[i]];
    }

    uint32_t v = order;
    while (a[v] == v) {
        --v;
    }
    for (uint32_t i = graphSize - 1; i > order + 1; --i) {
        if (a[i] == v) {
            a[i] = a[v];
            a[v] = i;
            --v;
            while (a[v] == v) {
                --v;
            }
        }
    }
    return;
}

/**
 * Transforms a graph from cross or beginpointer representation to
 * swapped cross or beginpointer representation
 * @param a graph in cross or beginpointer representation
 */
void Graphrepresentations::swapRepresentation(uint32_t* a) {
    uint32_t order = a[0];
    for (uint32_t i = 1; i <= order; ++i) {
        uint32_t temp = a[a[i]];
        a[a[i]] = i;
        a[i] = temp;
    }
    return;
}

void Graphrepresentations::standardToShifted(unsigned int* g) {
    unsigned int order = g[0];
    unsigned int wordsize = sizeof(*g) * 8;
    unsigned int c = static_cast<unsigned int>(log2(order / wordsize) - 1);
    unsigned int c_prime = c + 1;
    unsigned int wordsize_packed = wordsize - c_prime;
    unsigned int c_prime_mask = static_cast<unsigned int>(-1)
                                << (wordsize - c_prime);
    unsigned int current_prefix = g[1] & c_prime_mask;
    std::vector<unsigned int> changing_positions;
    for (unsigned int i = 1; i <= order; ++i) {
        if ((g[i] & c_prime_mask) != current_prefix) {
            unsigned int num_increments =
                ((g[i] & c_prime_mask) - current_prefix) >>
                (wordsize - c_prime);
            current_prefix = g[i] & c_prime_mask;
            for (unsigned int j = 0; j < num_increments; ++j) {
                changing_positions.push_back(i);
            }
        }
    }
    for (unsigned int i = 1; i <= order; ++i) {
        unsigned int value = g[i] & ~c_prime_mask;
        unsigned int startbit = wordsize_packed * (i - 1);
        unsigned int offset = startbit & (wordsize - 1);
        unsigned int index = (startbit >> (unsigned int)log2(wordsize)) + 1;
        if (offset + wordsize_packed > wordsize) {
            unsigned int leftbits = wordsize - offset;
            unsigned int rightbits = wordsize_packed - leftbits;
            g[index] = (value >> rightbits) | g[index];
            g[index + 1] = value << (leftbits + c_prime);
        } else {
            g[index] = (g[index] & ~((unsigned int)-1 >> offset)) |
                       (value << (c_prime - offset));
        }
    }
    unsigned int changing_positions_index =
        order - static_cast<unsigned int>(pow(2, c_prime));
    for (unsigned int pos : changing_positions) {
        g[changing_positions_index++] = pos;
    }
    g[changing_positions_index] = 0;
    g[g[0]] = c_prime;
}

void Graphrepresentations::shiftedToStandard(unsigned int* g) {
    unsigned int order = g[0];
    unsigned int wordsize = sizeof(g[0]) * 8;
    unsigned int c_prime = g[order];
    unsigned int wordsize_packed = wordsize - c_prime;
    unsigned int c_prime_mask = ~0u << (wordsize - c_prime);
    unsigned int initial_prefix = order + 2 & c_prime_mask;
    std::vector<unsigned int> changing_positions;
    uint i = order - (1 << c_prime);
    while (g[i] < g[i + 1]) {
        changing_positions.push_back(g[i++]);
    }
    changing_positions.push_back(g[i]);
    unsigned int changing_pos = changing_positions.size() - 1;
    unsigned int current_prefix =
        (initial_prefix >> (wordsize - c_prime)) + changing_positions.size();
    for (unsigned int j = order; j > 0; --j) {
        while (changing_pos >= 0 && changing_positions[changing_pos] == j + 1) {
            changing_pos--;
            current_prefix--;
        }
        unsigned int startbit = wordsize_packed * (i - 1);
        unsigned int offset = startbit & (wordsize - 1);
        unsigned int index = (startbit >> (unsigned int)log2(wordsize)) + 1;
        if (offset + wordsize_packed > wordsize) {
            unsigned int leftbits = wordsize - offset;
            unsigned int rightbits = wordsize_packed - leftbits;
            unsigned int value = (g[index] << rightbits) |
                                 (g[index + 1] >> (wordsize - rightbits));
            g[i] = (value & ~c_prime_mask) |
                   (current_prefix << (wordsize - c_prime));
        } else {
            g[i] = ((g[index] >> (c_prime - offset)) & ~c_prime_mask) |
                   (current_prefix << (wordsize - c_prime));
        }
    }
}
