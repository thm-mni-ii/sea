#include "sealib/iterator/dfs.h"
#include <math.h>
#include <memory>
#include <sstream>
#include "./inplacerunner.h"

namespace Sealib {

void DFS::visit_standard(uint64_t u0, Graph const *g, std::vector<uint64_t> *color,
                         std::vector<std::pair<uint64_t, uint64_t>> *s,
                         Consumer preprocess, BiConsumer preexplore,
                         BiConsumer postexplore, Consumer postprocess) {
    s->push_back({u0, 0});
    while (!s->empty()) {
        std::pair<uint64_t, uint64_t> x = s->back();
        s->pop_back();
        uint64_t u = x.first;
        uint64_t k = x.second;
        if (color->operator[](u) == DFS_WHITE) {
            preprocess(u);
            color->operator[](u) = DFS_GRAY;
        }
        if (k < g->deg(u)) {
            s->push_back({u, k + 1});
            uint64_t v = g->head(u, k);
            preexplore(u, v);
            if (color->operator[](v) == DFS_WHITE) {
                s->push_back({v, 0});
            } else {
                postexplore(u, v);
            }
        } else {
            color->operator[](u) = DFS_BLACK;
            postprocess(u);
            if (u != u0) {
                uint64_t pu = s->back().first;
                postexplore(pu, u);
            }
        }
    }
}

void DFS::visit_nloglogn(uint64_t u0, Graph const *g, CompactArray *color,
                         SegmentStack *s,
                         std::function<void(uint64_t u)> restoration,
                         Consumer preprocess, BiConsumer preexplore,
                         BiConsumer postexplore, Consumer postprocess) {
    s->push(std::pair<uint64_t, uint64_t>(u0, 0));
    std::pair<uint64_t, uint64_t> x;
    while (!s->isEmpty()) {
        uint8_t sr = s->pop(&x);
        if (sr == DFS_DO_RESTORE) {
            restoration(u0);
            s->pop(&x);
        } else if (sr == DFS_NO_MORE_NODES) {
            return;
        }
        uint64_t u, k;
        u = x.first;
        k = x.second;
        if (color->get(u) == DFS_WHITE) {
            preprocess(u);
            color->insert(u, DFS_GRAY);
        }
        if (k < g->deg(u)) {
            s->push(std::pair<uint64_t, uint64_t>(u, k + 1));
            uint64_t v = g->head(u, k);
            preexplore(u, v);
            if (color->get(v) == DFS_WHITE) {
                s->push(std::pair<uint64_t, uint64_t>(v, 0));
            } else {
                postexplore(u, v);
            }
        } else {
            if (u != u0) {
                std::pair<uint64_t, uint64_t> px;
                sr = s->pop(&px);
                if (sr == DFS_DO_RESTORE) {
                    restoration(u0);
                    s->pop(&px);
                }
                uint64_t pu = px.first;
                postexplore(pu, u);
                s->push(px);
            }
            color->insert(u, DFS_BLACK);
            postprocess(u);
        }
    }
}

void DFS::restore_full(uint64_t u0, Graph const *g, CompactArray *color,
                       BasicSegmentStack *s) {
    s->saveTrailer();
    s->dropAll();
    for (uint64_t a = 0; a < g->getOrder(); a++) {
        if (color->get(a) == DFS_GRAY) {
            color->insert(a, DFS_WHITE);
        }
    }
    s->push(std::pair<uint64_t, uint64_t>(u0, 0));
    std::pair<uint64_t, uint64_t> x;
    while (!s->isAligned()) {
        s->pop(&x);
        uint64_t u = x.first, k = x.second;
        if (color->get(u) == DFS_WHITE) color->insert(u, DFS_GRAY);
        if (k < g->deg(u)) {
            s->push(std::pair<uint64_t, uint64_t>(u, k + 1));
            if (s->isAligned()) break;
            uint64_t v = g->head(u, k);
            if (color->get(v) == DFS_WHITE)
                s->push(std::pair<uint64_t, uint64_t>(v, 0));
        }
    }
}

/**
 * Get the outgoing edge of u which leads to a gray, top-segment node.
 * @param u node to check
 * @param k starting edge to check
 * @return (c,k) a result tuple with
 *    c: are there more possible nodes to find?
 *    k: outgoing edge that points to a gray, top-segment node (if c is false, k
 * is an 'enclosing' edge)
 */
static std::pair<bool, uint64_t> findEdge(const uint64_t u, const uint64_t k,
                                      Graph const *g, CompactArray *c,
                                      ExtendedSegmentStack *s) {
    std::pair<bool, uint64_t> r = std::make_pair(false, static_cast<uint64_t>(-1));
    for (uint64_t i = k; i < g->deg(u); i++) {
        uint64_t v = g->head(u, i);
        if (c->get(v) == DFS_GRAY && s->isInTopSegment(v, true)) {
            r = std::make_pair(true, i);
            break;
        }
    }
    if (!r.first) {
        std::pair<uint64_t, uint64_t> a;
        s->getTopTrailer(&a);
        r = std::make_pair(false, a.second - 1);
    }
    return r;
}

void DFS::restore_top(uint64_t u0, Graph const *g, CompactArray *color,
                      ExtendedSegmentStack *s) {
    std::pair<uint64_t, uint64_t> x;
    uint64_t u = u0, k = 0;
    if (s->getRestoreTrailer(&x) == 1) {
        color->insert(u, DFS_WHITE);
    } else {
        u = x.first, k = x.second - 1;
        u = g->head(u, k), k = s->getOutgoingEdge(u);
        color->insert(u, DFS_WHITE);
    }
    while (!s->isAligned()) {
        std::pair<bool, uint64_t> r = findEdge(u, k, g, color, s);
        uint64_t u1 = u, k1 = r.second;
        if (r.first) {
            s->push(std::pair<uint64_t, uint64_t>(
                u1, k1 + 1));  // k+1 to simulate the normal stack behaviour
            u = g->head(u1, k1);
            k = s->getOutgoingEdge(u);
            color->insert(u, DFS_WHITE);
        } else {
            s->push(std::pair<uint64_t, uint64_t>(u1, k1 + 1));
            // restoration loop must end now, the stack is aligned
        }
    }
    s->recolorLow(DFS_GRAY);
}

void DFS::visit_nplusm(uint64_t u0, UndirectedGraph const *g, CompactArray *color,
                       Sequence<uint64_t> *back, Consumer preprocess,
                       BiConsumer preexplore, BiConsumer postexplore,
                       Consumer postprocess) {
    color->insert(u0, DFS_GRAY);
    preprocess(u0);
    uint64_t u = u0, k = 0;
    while (true) {
        if (k < g->deg(u)) {
            uint64_t v = g->head(u, k);
            preexplore(u, k);
            if (color->get(v) == DFS_WHITE) {
                preprocess(v);
                color->insert(v, DFS_GRAY);
                back->insert(v, g->mate(u, k));
                u = v;
                k = 0;
                continue;
            } else {
                postexplore(u, k);
                k++;
            }
        } else {
            color->insert(u, DFS_BLACK);
            postprocess(u);
            if (u != u0) {
                uint64_t pk = g->mate(u, static_cast<uint64_t>(back->get(u))),
                     pu = g->head(u, static_cast<uint64_t>(back->get(u)));
                postexplore(pu, pk);
                u = pu;
                k = pk + 1;
            } else {
                break;
            }
        }
    }
}

void DFS::standardDFS(Graph const *g, Consumer preprocess,
                      BiConsumer preexplore, BiConsumer postexplore,
                      Consumer postprocess) {
    std::vector<uint64_t> color(g->getOrder());
    std::vector<std::pair<uint64_t, uint64_t>> s;
    for (uint64_t u = 0; u < g->getOrder(); u++) {
        if (color[u] == DFS_WHITE) {
            visit_standard(u, g, &color, &s, preprocess, preexplore,
                           postexplore, postprocess);
        }
    }
}

void DFS::nBitDFS(Graph const *g, Consumer preprocess, BiConsumer preexplore,
                  BiConsumer postexplore, Consumer postprocess) {
    uint64_t n = g->getOrder();
    double e = 0.2;
    uint64_t q = static_cast<uint64_t>(ceil(
        ceil(e / 6 * n) / (8 * sizeof(std::pair<uint64_t, uint64_t>))));  // 2q entries
                                                                  // on S shall
                                                                  // take up at
                                                                  // most (e/3)n
                                                                  // bits
    uint64_t qs = 3;  // stable segment size (?)
    if (q < qs) q = qs;

    // printf("e=%3.2f, q=%u, n=%u\n", e, q, n);
    BasicSegmentStack s(q);
    CompactArray color(n, 3);
    for (uint64_t a = 0; a < n; a++) color.insert(a, DFS_WHITE);
    for (uint64_t a = 0; a < n; a++) {
        if (color.get(a) == DFS_WHITE)
            visit_nloglogn(a, g, &color, &s,
                           [&](uint64_t u0) { restore_full(u0, g, &color, &s); },
                           preprocess, preexplore, postexplore, postprocess);
    }
}

void DFS::nloglognBitDFS(Graph const *g, Consumer preprocess,
                         BiConsumer preexplore, BiConsumer postexplore,
                         Consumer postprocess) {
    uint64_t n = g->getOrder();
    CompactArray color(n, 3);
    ExtendedSegmentStack s(n, g, &color);
    for (uint64_t a = 0; a < n; a++) color.insert(a, DFS_WHITE);
    for (uint64_t a = 0; a < n; a++) {
        if (color.get(a) == DFS_WHITE)
            visit_nloglogn(a, g, &color, &s,
                           [&](uint64_t u0) { restore_top(u0, g, &color, &s); },
                           preprocess, preexplore, postexplore, postprocess);
    }
}

void DFS::nplusmBitDFS(UndirectedGraph const *g, Consumer preprocess,
                       BiConsumer preexplore, BiConsumer postexplore,
                       Consumer postprocess) {
    uint64_t n = g->getOrder();
    CompactArray color(n, 3);
    for (uint64_t a = 0; a < n; a++) color.insert(a, DFS_WHITE);
    std::vector<bool> bits;
    for (uint64_t u = 0; u < n; u++) {
        bits.push_back(1);
        for (uint64_t k = 0; k < ceil(log2(g->deg(u) + 1)); k++) {
            bits.push_back(0);
        }
    }
    StaticSpaceStorage back(bits);
    for (uint64_t a = 0; a < n; a++) {
        if (color.get(a) == DFS_WHITE)
            visit_nplusm(a, g, &color, &back, preprocess, preexplore,
                         postexplore, postprocess);
    }
}

void DFS::runLinearTimeInplaceDFS(uint64_t *graph, Consumer preprocess,
                                  Consumer postprocess, uint64_t startVertex) {
    auto *ilDFSRunner =
        new LinearTimeInplaceDFSRunner(graph, preprocess, postprocess);
    ilDFSRunner->run(startVertex);
    delete ilDFSRunner;
}
}  // namespace Sealib
