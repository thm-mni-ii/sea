#include "sealib/iterator/dfs.h"
#include <math.h>
#include <sstream>
#include <stack>
#include <vector>
#include "./inplacerunner.h"

namespace Sealib {

void DFS::process_standard(uint u0, Graph *g, uint *color, Consumer preProcess,
                           BiConsumer preExplore, BiConsumer postExplore,
                           Consumer postProcess) {
    std::stack<std::pair<uint, uint>> *s =
        new std::stack<std::pair<uint, uint>>;
    s->push(std::pair<uint, uint>(u0, 0));
    while (!s->empty()) {
        std::pair<uint, uint> x = s->top();
        s->pop();
        uint u = x.first;
        uint k = x.second;
        if (color[u] == DFS_WHITE) {
            preProcess(u);
            color[u] = DFS_GRAY;
        }
        if (k < g->getNodeDegree(u)) {
            s->push(std::pair<uint, uint>(u, k + 1));
            uint v = g->head(u, k);
            preExplore(u, v);
            if (color[v] == DFS_WHITE) {
                s->push(std::pair<uint, uint>(v, 0));
            } else {
                postExplore(u, v);
            }
        } else {
            color[u] = DFS_BLACK;
            postProcess(u);
            if (u != u0) {
                uint pu = s->top().first;
                postExplore(pu, u);
            }
        }
    }
    delete s;
}

template <class SS>
void DFS::process_small(uint u0, Graph *g, CompactArray *color, SS *s,
                        void (*restoration)(uint, Graph *, CompactArray *,
                                            SS *),
                        Consumer preProcess, BiConsumer preExplore,
                        BiConsumer postExplore, Consumer postProcess) {
    s->push(std::pair<uint, uint>(u0, 0));
    std::pair<uint, uint> x;
    while (!s->isEmpty()) {
        int sr = s->pop(&x);
        if (sr == DFS_DO_RESTORE) {
            restoration(u0, g, color, s);
            s->pop(&x);
        } else if (sr == DFS_NO_MORE_NODES) {
            return;
        }
        uint u, k;
        u = x.first;
        k = x.second;
        if (color->get(u) == DFS_WHITE) {
            preProcess(u);
            color->insert(u, DFS_GRAY);
        }
        if (k < g->getNodeDegree(u)) {
            s->push(std::pair<uint, uint>(u, k + 1));
            uint v = g->head(u, k);
            preExplore(u, v);
            if (color->get(v) == DFS_WHITE) {
                s->push(std::pair<uint, uint>(v, 0));
            } else {
                postExplore(u, v);
            }
        } else {
            if (u != u0) {
                std::pair<uint, uint> px;
                sr = s->pop(&px);
                if (sr == DFS_DO_RESTORE) {
                    restoration(u0, g, color, s);
                    s->pop(&px);
                }
                uint pu = px.first;
                postExplore(pu, u);
                s->push(px);
            }
            color->insert(u, DFS_BLACK);
            postProcess(u);
        }
    }
}

void DFS::restore_full(uint u0, Graph *g, CompactArray *color,
                       BasicSegmentStack *s) {
    s->saveTrailer();
    s->dropAll();
    for (uint a = 0; a < g->getOrder(); a++) {
        if (color->get(a) == DFS_GRAY) {
            color->insert(a, DFS_WHITE);
        }
    }
    s->push(std::pair<uint, uint>(u0, 0));
    std::pair<uint, uint> x;
    while (!s->isAligned()) {
        s->pop(&x);
        uint u = x.first, k = x.second;
        if (color->get(u) == DFS_WHITE) color->insert(u, DFS_GRAY);
        if (k < g->getNodeDegree(u)) {
            s->push(std::pair<uint, uint>(u, k + 1));
            if (s->isAligned()) break;
            uint v = g->head(u, k);
            if (color->get(v) == DFS_WHITE)
                s->push(std::pair<uint, uint>(v, 0));
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
static std::pair<bool, uint> findEdge(const uint u, const uint k, Graph *g,
                                      CompactArray *c,
                                      ExtendedSegmentStack *s) {
    std::pair<bool, uint> r = std::make_pair(false, static_cast<uint>(-1));
    for (uint i = k; i < g->getNodeDegree(u); i++) {
        uint v = g->head(u, i);
        if (c->get(v) == DFS_GRAY && s->isInTopSegment(v, true)) {
            r = std::make_pair(true, i);
            break;
        }
    }
    if (!r.first) {
        std::pair<uint, uint> a;
        s->getTopTrailer(&a);
        r = std::make_pair(false, a.second - 1);
    }
    return r;
}

void DFS::restore_top(uint u0, Graph *g, CompactArray *color,
                      ExtendedSegmentStack *s) {
    std::pair<uint, uint> x;
    uint u = u0, k = 0;
    if (s->getRestoreTrailer(&x) == 1) {
        color->insert(u, DFS_WHITE);
    } else {
        u = x.first, k = x.second - 1;
        u = g->head(u, k), k = s->getOutgoingEdge(u);
        color->insert(u, DFS_WHITE);
    }
    while (!s->isAligned()) {
        std::pair<bool, uint> r = findEdge(u, k, g, color, s);
        uint u1 = u, k1 = r.second;
        if (r.first) {
            s->push(std::pair<uint, uint>(
                u1, k1 + 1));  // k+1 to simulate the normal stack behaviour
            u = g->head(u1, k1);
            k = s->getOutgoingEdge(u);
            color->insert(u, DFS_WHITE);
        } else {
            s->push(std::pair<uint, uint>(u1, k1 + 1));
            // restoration loop must end now, the stack is aligned
        }
    }
    s->recolorLow(DFS_GRAY);
}

template <class S>
void DFS::process_static(uint u0, UndirectedGraph *g, CompactArray *color,
                         S *back, Consumer preprocess, BiConsumer preexplore,
                         BiConsumer postexplore, Consumer postprocess) {
    color->insert(u0, DFS_GRAY);
    back->insert(u0, std::numeric_limits<uint>::max());
    preprocess(u0);
    uint u = u0, k = 0;
    while (true) {
        if (k < g->getNodeDegree(u)) {
            uint v = g->head(u, k);
            preexplore(u, k);
            if (color->get(v) == DFS_WHITE) {
                preprocess(v);
                color->insert(v, DFS_GRAY);
                back->insert(v, std::get<1>(g->mate(u, k)));
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
                std::tuple<uint, uint> p =
                    g->mate(u, static_cast<uint>(back->get(u)));
                uint pu = std::get<0>(p), pk = std::get<1>(p);
                postexplore(pu, pk);
                u = pu;
                k = pk + 1;
            } else {
                break;
            }
        }
    }
}

void DFS::standardDFS(Graph *g, Consumer preProcess, BiConsumer preExplore,
                      BiConsumer postExplore, Consumer postProcess) {
    uint *color = new uint[g->getOrder()];
    for (uint a = 0; a < g->getOrder(); a++) color[a] = DFS_WHITE;
    for (uint u = 0; u < g->getOrder(); u++) {
        if (color[u] == DFS_WHITE) {
            process_standard(u, g, color, preProcess, preExplore, postExplore,
                             postProcess);
        }
    }
    delete[] color;
}

void DFS::nBitDFS(Graph *g, Consumer preProcess, BiConsumer preExplore,
                  BiConsumer postExplore, Consumer postProcess) {
    uint32_t n = g->getOrder();
    double e = 0.2;
    unsigned q = static_cast<unsigned>(ceil(
        ceil(e / 6 * n) / (8 * sizeof(std::pair<uint, uint>))));  // 2q entries
                                                                  // on S shall
                                                                  // take up at
                                                                  // most (e/3)n
                                                                  // bits
    unsigned qs = 3;  // stable segment size (?)
    if (q < qs) q = qs;

    // printf("e=%3.2f, q=%u, n=%u\n", e, q, n);
    BasicSegmentStack s(q);
    CompactArray color(n, 3);
    for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
    for (uint a = 0; a < n; a++) {
        if (color.get(a) == DFS_WHITE)
            process_small(a, g, &color, &s, restore_full, preProcess,
                          preExplore, postExplore, postProcess);
    }
}

void DFS::nloglognBitDFS(Graph *g, Consumer preProcess, BiConsumer preExplore,
                         BiConsumer postExplore, Consumer postProcess) {
    uint32_t n = g->getOrder();
    CompactArray color(n, 3);
    ExtendedSegmentStack s(n, g, &color);
    for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
    for (uint a = 0; a < n; a++) {
        if (color.get(a) == DFS_WHITE)
            process_small(a, g, &color, &s, restore_top, preProcess, preExplore,
                          postExplore, postProcess);
    }
}

void DFS::nplusmBitDFS(UndirectedGraph *g, Consumer preprocess,
                       BiConsumer preexplore, BiConsumer postexplore,
                       Consumer postprocess) {
    uint32_t n = g->getOrder();
    CompactArray color(n, 3);
    for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
    std::vector<bool> bits;
    for (uint u = 0; u < n; u++) {
        bits.push_back(1);
        for (uint k = 0; k < ceil(log2(g->getNodeDegree(u) + 1)); k++) {
            bits.push_back(0);
        }
    }
    StaticSpaceStorage back(bits);
    for (uint a = 0; a < n; a++) {
        if (color.get(a) == DFS_WHITE)
            process_static(a, g, &color, &back, preprocess, preexplore,
                           postexplore, postprocess);
    }
}

void DFS::runLinearTimeInplaceDFS(uint *graph, Consumer preProcess,
                                  Consumer postProcess, uint startVertex) {
    auto *ilDFSRunner =
        new LinearTimeInplaceDFSRunner(graph, preProcess, postProcess);
    ilDFSRunner->run(startVertex);
    delete ilDFSRunner;
}
}  // namespace Sealib
