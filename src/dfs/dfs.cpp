#include "sealib/iterator/dfs.h"
#include <math.h>
#include <memory>
#include <sstream>
#include "./inplacerunner.h"

namespace Sealib {

void DFS::visit_standard(uint64_t u0, Graph const &g,
                         std::vector<uint64_t> *color,
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
        if (k < g.deg(u)) {
            s->push_back({u, k + 1});
            uint64_t v = g.head(u, k);
            preexplore(u, k);
            if (color->operator[](v) == DFS_WHITE) {
                s->push_back({v, 0});
            } else {
                postexplore(u, k);
            }
        } else {
            color->operator[](u) = DFS_BLACK;
            postprocess(u);
            if (u != u0) {
                std::pair<uint64_t, uint64_t> p = s->back();
                postexplore(p.first, p.second - 1);
            }
        }
    }
}

void DFS::visit_nloglogn(
    uint64_t u0, Graph const &g, CompactArray *color, SegmentStack *s,
    std::function<void(uint64_t, Graph const &, CompactArray *, SegmentStack *)>
        restoration,
    Consumer preprocess, BiConsumer preexplore, BiConsumer postexplore,
    Consumer postprocess) {
    s->push({u0, 0});
    std::pair<uint64_t, uint64_t> x;
    while (!s->isEmpty()) {
        uint8_t sr = s->pop(&x);
        if (sr == DFS_DO_RESTORE) {
            restoration(u0, g, color, s);
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
        if (k < g.deg(u)) {
            s->push({u, k + 1});
            uint64_t v = g.head(u, k);
            preexplore(u, k);
            if (color->get(v) == DFS_WHITE) {
                s->push({v, 0});
            } else {
                postexplore(u, k);
            }
        } else {
            color->insert(u, DFS_BLACK);
            postprocess(u);
            if (u != u0) {
                std::pair<uint64_t, uint64_t> px;
                sr = s->pop(&px);
                if (sr == DFS_DO_RESTORE) {
                    restoration(u0, g, color, s);
                    s->pop(&px);
                }
                postexplore(px.first, px.second - 1);
                s->push(px);
            }
        }
    }
}

void DFS::restore_full(uint64_t u0, Graph const &g, CompactArray *color,
                       SegmentStack *ps) {
    BasicSegmentStack *s = reinterpret_cast<BasicSegmentStack *>(ps);
    s->saveTrailer();
    s->dropAll();
    for (uint64_t a = 0; a < g.getOrder(); a++) {
        if (color->get(a) == DFS_GRAY) {
            color->insert(a, DFS_WHITE);
        }
    }
    s->push({u0, 0});
    std::pair<uint64_t, uint64_t> x;
    while (!s->isAligned()) {
        s->pop(&x);
        uint64_t u = x.first, k = x.second;
        if (color->get(u) == DFS_WHITE) color->insert(u, DFS_GRAY);
        if (k < g.deg(u)) {
            s->push({u, k + 1});
            if (s->isAligned()) break;
            uint64_t v = g.head(u, k);
            if (color->get(v) == DFS_WHITE) s->push({v, 0});
        }
    }
}

void DFS::restore_top(uint64_t u0, Graph const &g, CompactArray *color,
                      SegmentStack *ps) {
    ExtendedSegmentStack *s = reinterpret_cast<ExtendedSegmentStack *>(ps);
    std::pair<uint64_t, uint64_t> x;
    uint64_t u = u0, k = 0;
    if (s->getRestoreTrailer(&x) == 1) {
        color->insert(u, DFS_WHITE);
    } else {
        u = x.first, k = x.second - 1;
        u = g.head(u, k), k = s->getOutgoingEdge(u);
        color->insert(u, DFS_WHITE);
    }
    while (!s->isAligned()) {
        std::pair<bool, uint64_t> r = s->findEdge(u, k);
        uint64_t u1 = u, k1 = r.second;
        if (r.first) {
            s->push(
                {u1, k1 + 1});  // k+1 to simulate the normal stack behaviour
            u = g.head(u1, k1);
            k = s->getOutgoingEdge(u);
            color->insert(u, DFS_WHITE);
        } else {
            s->push({u1, k1 + 1});
            // restoration loop must end now, the stack is aligned
        }
    }
    s->recolorLow(DFS_GRAY);
}

void DFS::visit_nplusm(uint64_t u0, UndirectedGraph const &g,
                       CompactArray *color, Sequence<uint64_t> *parent,
                       Consumer preprocess, BiConsumer preexplore,
                       BiConsumer postexplore, Consumer postprocess) {
    color->insert(u0, DFS_GRAY);
    preprocess(u0);
    uint64_t u = u0, k = 0;
    while (true) {
        if (k < g.deg(u)) {
            uint64_t v = g.head(u, k);
            preexplore(u, k);
            if (color->get(v) == DFS_WHITE) {
                preprocess(v);
                color->insert(v, DFS_GRAY);
                parent->insert(v, g.mate(u, k));
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
                uint64_t pk = g.mate(u, parent->get(u)),
                         pu = g.head(u, parent->get(u));
                postexplore(pu, pk);
                u = pu;
                k = pk + 1;
            } else {
                break;
            }
        }
    }
}

void DFS::standardDFS(Graph const &g, Consumer preprocess,
                      BiConsumer preexplore, BiConsumer postexplore,
                      Consumer postprocess) {
    std::vector<uint64_t> color(g.getOrder());
    std::vector<std::pair<uint64_t, uint64_t>> s;
    for (uint64_t u = 0; u < g.getOrder(); u++) {
        if (color[u] == DFS_WHITE) {
            visit_standard(u, g, &color, &s, preprocess, preexplore,
                           postexplore, postprocess);
        }
    }
}

void DFS::nBitDFS(Graph const &g, Consumer preprocess, BiConsumer preexplore,
                  BiConsumer postexplore, Consumer postprocess) {
    uint64_t n = g.getOrder();
    double e = 0.2;
    uint64_t q = static_cast<uint64_t>(
        ceil(ceil(e / 6 * n) /
             (8 * sizeof(std::pair<uint64_t, uint64_t>))));  // 2q entries
                                                             // on S shall
                                                             // take up at
                                                             // most (e/3)n
                                                             // bits
    uint64_t qs = 3;  // stable segment size (?)
    if (q < qs) q = qs;

    BasicSegmentStack s(q);
    CompactArray color(n, 3);
    for (uint64_t a = 0; a < n; a++) {
        if (color.get(a) == DFS_WHITE)
            visit_nloglogn(a, g, &color, &s, restore_full, preprocess,
                           preexplore, postexplore, postprocess);
    }
}

void DFS::nloglognBitDFS(Graph const &g, Consumer preprocess,
                         BiConsumer preexplore, BiConsumer postexplore,
                         Consumer postprocess) {
    uint64_t n = g.getOrder();
    CompactArray color(n, 3);
    ExtendedSegmentStack s(n, g, &color);
    for (uint64_t a = 0; a < n; a++) {
        if (color.get(a) == DFS_WHITE)
            visit_nloglogn(a, g, &color, &s, restore_top, preprocess,
                           preexplore, postexplore, postprocess);
    }
}

void DFS::nplusmBitDFS(UndirectedGraph const &g, Consumer preprocess,
                       BiConsumer preexplore, BiConsumer postexplore,
                       Consumer postprocess) {
    uint64_t n = g.getOrder();
    CompactArray color(n, 3);
    StaticSpaceStorage parent(g);
    for (uint64_t a = 0; a < n; a++) {
        if (color.get(a) == DFS_WHITE)
            visit_nplusm(a, g, &color, &parent, preprocess, preexplore,
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
