#include "sealib/iterator/dfs.h"
#include <math.h>
#include <sstream>
#include <stack>
#include <vector>
#include "./inplacerunner.h"
#include "./segmentstack.h"

using namespace Sealib;  // NOLINT

static void process_standard(uint u0, Graph *g, uint *color,
                             UserFunc1 preProcess, UserFunc2 preExplore,
                             UserFunc2 postExplore, UserFunc1 postProcess) {
    std::stack<Pair> *s = new std::stack<Pair>;
    s->push(Pair(u0, 0));
    while (!s->empty()) {
        Pair x = s->top();
        s->pop();
        uint u = x.head();
        uint k = x.tail();
        if (color[u] == DFS_WHITE) {
            if (preProcess != DFS_NOP_PROCESS) preProcess(u);
            color[u] = DFS_GRAY;
        }
        if (k < g->getNodeDegree(u)) {
            s->push(Pair(u, k + 1));
            uint v = g->head(u, k);
            if (preExplore != DFS_NOP_EXPLORE) preExplore(u, v);
            if (color[v] == DFS_WHITE) {
                s->push(Pair(v, 0));
            } else {
                if (postExplore != DFS_NOP_EXPLORE) postExplore(u, v);
            }
        } else {
            color[u] = DFS_BLACK;
            if (postProcess != DFS_NOP_PROCESS) postProcess(u);
            if (postExplore != DFS_NOP_EXPLORE && u != u0) {
                uint pu = s->top().head();
                postExplore(pu, u);
            }
        }
    }
    delete s;
}

template <class SS>
static void process_small(uint u0, Graph *g, CompactArray *color, SS *s,
                          void (*restoration)(uint, Graph *, CompactArray *,
                                              SS *),
                          UserFunc1 preProcess, UserFunc2 preExplore,
                          UserFunc2 postExplore, UserFunc1 postProcess) {
    s->push(Pair(u0, 0));
    Pair x;
    while (!s->isEmpty()) {
        int sr = s->pop(&x);
        if (sr == DFS_DO_RESTORE) {
            restoration(u0, g, color, s);
            s->pop(&x);
        } else if (sr == DFS_NO_MORE_NODES) {
            return;
        }
        uint u, k;
        u = x.head();
        k = x.tail();
        if (color->get(u) == DFS_WHITE) {
            if (preProcess != DFS_NOP_PROCESS) preProcess(u);
            color->insert(u, DFS_GRAY);
        }
        if (k < g->getNodeDegree(u)) {
            s->push(Pair(u, k + 1));
            uint v = g->head(u, k);
            if (preExplore != DFS_NOP_EXPLORE) preExplore(u, v);
            if (color->get(v) == DFS_WHITE) {
                s->push(Pair(v, 0));
            } else {
                if (postExplore != DFS_NOP_EXPLORE) postExplore(u, v);
            }
        } else {
            if (postExplore != DFS_NOP_EXPLORE && u != u0) {
                Pair px;
                sr = s->pop(&px);
                if (sr == DFS_DO_RESTORE) {
                    restoration(u0, g, color, s);
                    s->pop(&px);
                }
                uint pu = px.head();
                postExplore(pu, u);
                s->push(px);
            }
            color->insert(u, DFS_BLACK);
            if (postProcess != DFS_NOP_PROCESS) postProcess(u);
        }
    }
}

static void restore_full(uint u0, Graph *g, CompactArray *color,
                         BasicSegmentStack *s) {
    s->saveTrailer();
    s->dropAll();
    for (uint a = 0; a < g->getOrder(); a++) {
        if (color->get(a) == DFS_GRAY) {
            color->insert(a, DFS_WHITE);
        }
    }
    s->push(Pair(u0, 0));
    Pair x;
    while (!s->isAligned()) {
        s->pop(&x);
        uint u = x.head(), k = x.tail();
        if (color->get(u) == DFS_WHITE) color->insert(u, DFS_GRAY);
        if (k < g->getNodeDegree(u)) {
            s->push(Pair(u, k + 1));
            if (s->isAligned()) break;
            uint v = g->head(u, k);
            if (color->get(v) == DFS_WHITE) s->push(Pair(v, 0));
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
        Pair a;
        s->getTopTrailer(&a);
        r = std::make_pair(false, a.tail() - 1);
    }
    return r;
}

static void restore_top(uint u0, Graph *g, CompactArray *color,
                        ExtendedSegmentStack *s) {
    Pair x;
    uint u = u0, k = 0;
    if (s->getRestoreTrailer(&x) == 1) {
        color->insert(u, DFS_WHITE);
    } else {
        u = x.head(), k = x.tail() - 1;
        u = g->head(u, k), k = s->getOutgoingEdge(u);
        color->insert(u, DFS_WHITE);
    }
    while (!s->isAligned()) {
        std::pair<bool, uint> r = findEdge(u, k, g, color, s);
        uint u1 = u, k1 = r.second;
        if (r.first) {
            s->push(Pair(
                u1, k1 + 1));  // k+1 to simulate the normal stack behaviour
            u = g->head(u1, k1);
            k = s->getOutgoingEdge(u);
            color->insert(u, DFS_WHITE);
        } else {
            s->push(Pair(u1, k1 + 1));
            // restoration loop must end now, the stack is aligned
        }
    }
    s->recolorLow(DFS_GRAY);
}

template <class C>
static void process_static(uint u0, UndirectedGraph *g, CompactArray *color, C *back,
                           UserFunc1 preprocess, UserFunc2 preexplore,
                           UserFunc2 postexplore, UserFunc1 postprocess) {
    color->insert(u0, DFS_GRAY);
    back->insert(u0, g->getNodeDegree(u0));
    if (preprocess != DFS_NOP_PROCESS) preprocess(u0);
    uint u = u0, k = 0;
    while (true) {
        if (k < g->getNodeDegree(u)) {
            uint v = g->head(u, k);
            if (preexplore != DFS_NOP_EXPLORE) preexplore(u, v);
            if (color->get(v) == DFS_WHITE) {
                if (preprocess != DFS_NOP_PROCESS) preprocess(v);
                color->insert(v, DFS_GRAY);
                back->insert(v, std::get<1>(g->mate(u, k)));
                u = v;
                k = 0;
                continue;
            } else {
                if (postexplore != DFS_NOP_EXPLORE) postexplore(u, v);
                k++;
            }
        } else {
            color->insert(u, DFS_BLACK);
            if (postprocess != DFS_NOP_PROCESS) postprocess(u);
            if (u != u0) {
                std::tuple<uint, uint> p =
                    g->mate(u, static_cast<uint>(back->get(u)));
                uint pu = std::get<0>(p), pk = std::get<1>(p);
                if (postexplore != DFS_NOP_EXPLORE) postexplore(pu, u);
                u = pu;
                k = pk + 1;
            } else {
                break;
            }
        }
    }
}

void DFS::standardDFS(Graph *g, UserFunc1 preProcess, UserFunc2 preExplore,
                      UserFunc2 postExplore, UserFunc1 postProcess) {
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

void DFS::nBitDFS(Graph *g, UserFunc1 preProcess, UserFunc2 preExplore,
                  UserFunc2 postExplore, UserFunc1 postProcess) {
  uint32_t n = g->getOrder();
  double e = 0.2;
  uint32_t q = static_cast<uint32_t>(ceil(
      ceil(e / 6 * n) /
      (8 *
       sizeof(Pair))));  // 2q entries on S shall take up at most (e/3)n bits
  uint32_t qs = 3;       // stable segment size (?)
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

void DFS::nloglognBitDFS(Graph *g, UserFunc1 preProcess, UserFunc2 preExplore,
                         UserFunc2 postExplore, UserFunc1 postProcess) {
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

void DFS::nplusmBitDFS(UndirectedGraph *g, UserFunc1 preprocess,
                       UserFunc2 preexplore, UserFunc2 postexplore,
                       UserFunc1 postprocess) {
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

void DFS::runLinearTimeInplaceDFS(uint *graph, UserFunc1 preProcess,
                                  UserFunc1 postProcess, uint startVertex) {
    auto *ilDFSRunner =
        new LinearTimeInplaceDFSRunner(graph, preProcess, postProcess);
    ilDFSRunner->run(startVertex);
    delete ilDFSRunner;
}
