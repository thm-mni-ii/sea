#include "sealib/dfs.h"
#include <math.h>
#include <iostream>
#include <sstream>
#include <stack>
#include "./inplacerunner.h"

using Sealib::DFS;
using Sealib::SegmentStack;
using Sealib::BasicSegmentStack;
using Sealib::ExtendedSegmentStack;
using Sealib::CompactArray;
using Sealib::Graph;
using Sealib::Node;

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
      preProcess(u);
      color[u] = DFS_GRAY;
    }
    if (k < g->getNodeDegree(u)) {
      s->push(Pair(u, k + 1));
      uint v = g->head(u, k);
      preExplore(u, v);
      if (color[v] == DFS_WHITE) {
        s->push(Pair(v, 0));
      } else {
        postExplore(u, v);
      }
    } else {
      if (u != u0) {
        uint pu = s->top().head();
        postExplore(pu, u);
      }
      color[u] = DFS_BLACK;
      postProcess(u);
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
      preProcess(u);
      color->insert(u, DFS_GRAY);
    }
    if (k < g->getNodeDegree(u)) {
      s->push(Pair(u, k + 1));
      uint v = g->head(u, k);
      preExplore(u, v);
      if (color->get(v) == DFS_WHITE) {
        s->push(Pair(v, 0));
      } else {
        postExplore(u, v);
      }
    } else {
      if (u != u0) {
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
      postProcess(u);
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
      s->push(Pair(u1, k1 + 1));  // k+1 to simulate the normal stack behaviour
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
  unsigned int n = g->getOrder();
  double e = 0.2;
  unsigned q = static_cast<unsigned>(ceil(
      ceil(e / 6 * n) /
      (8 *
       sizeof(Pair))));  // 2q entries on S shall take up at most (e/3)n bits
  unsigned qs = 3;       // stable segment size (?)
  if (q < qs) q = qs;

  // printf("e=%3.2f, q=%u, n=%u\n", e, q, n);
  BasicSegmentStack s(q);
  CompactArray color(n, 3);
  for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
  for (uint a = 0; a < n; a++) {
    if (color.get(a) == DFS_WHITE)
      process_small(a, g, &color, &s, restore_full, preProcess, preExplore,
                    postExplore, postProcess);
  }
}

void DFS::nloglognBitDFS(Graph *g, UserFunc1 preProcess, UserFunc2 preExplore,
                         UserFunc2 postExplore, UserFunc1 postProcess) {
  unsigned int n = g->getOrder();
  CompactArray color(n, 3);
  ExtendedSegmentStack s(n, g, &color);
  for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
  for (uint a = 0; a < n; a++) {
    if (color.get(a) == DFS_WHITE)
      process_small(a, g, &color, &s, restore_top, preProcess, preExplore,
                    postExplore, postProcess);
  }
}

void DFS::runLinearTimeInplaceDFS(uint *graph, UserFunc1 preProcess,
                                  UserFunc1 postProcess, uint startVertex) {
  auto *ilDFSRunner =
      new LinearTimeInplaceDFSRunner(graph, preProcess, postProcess);
  ilDFSRunner->run(startVertex);
  delete ilDFSRunner;
}

// reverse DFS:

DFS::ReverseDFS::ReverseDFS(Graph *graph)
    : g(graph),
      n(g->getOrder()),
      r(static_cast<uint>(ceil(8 * log2(n)))),
      w(static_cast<uint>(ceil(n / log2(n)))),
      c(new CompactArray(n, 3)),
      s(new ExtendedSegmentStack(n, g, c)),
      d(new CompactArray(n, r + 1)),
      f(new CompactArray(n, r + 1)),
      i(new IntervalData[r]),
      sp(w + 1) {
  std::cout << "r=" << r << ", w=" << w << "\n";
  for (uint a = 0; a < n; a++) {
    d->insert(a, r + 1);
    f->insert(a, r + 1);
  }
}

DFS::ReverseDFS::~ReverseDFS() {
  delete c;
  delete s;
  delete d;
  delete f;
  delete[] i;
}

void DFS::ReverseDFS::init() {
  for (uint a = 0; a < n; a++) c->insert(a, DFS_WHITE);
  for (uint a = 0; a < n; a++) {
    if (c->get(a) == DFS_WHITE) {
      process_small(a, g, c, s, restore_top,
                    [this](uint u) { storeBeginTime(u); }, DFS_NOP_EXPLORE,
                    DFS_NOP_EXPLORE, [this](uint u) { storeEndTime(u); });
    }
  }
}

bool DFS::ReverseDFS::more() { return static_cast<int>(ip) >= 0; }

DFS::UserCall DFS::ReverseDFS::next() {
  if (sp < w) {
    sp++;
    return seq.at(sp - 1);
  } else {  // build new sequence
    std::stack<Pair> sj = reconstructPart(ip, i[ip].hd, i[ip].h1);
    for (uint a = 0; a < n; a++) {
      if (f->get(a) < ip)
        c->insert(a, DFS_BLACK);
      else if (d->get(a) < ip)
        c->insert(a, DFS_GRAY);
      else
        c->insert(a, DFS_WHITE);
    }
    seq = simulate(sj, i[ip].h2);
    sp = 0;
    ip--;
    return seq.at(0);
  }
}

std::stack<Pair> DFS::ReverseDFS::reconstructPart(uint j, Pair from, Pair to) {
  std::cout << "reconstruct " << j << " " << from.head() << " " << to.head()
            << "\n";
  std::stack<Pair> rs;
  if (from.head() != ExtendedSegmentStack::INVALID) {
    rs.push(from);
    std::cout << rs.top().head() << " = " << to.head() << "?\n";
    while (!(rs.top() == to)) {
      Pair x = rs.top();
      uint u = g->head(x.head(), x.tail());
      for (uint k = 0; k < g->getNodeDegree(u); k++) {
        uint v = g->head(u, k);
        if (d->get(v) >= j) {
          std::cout << "restoring (" << u << "," << k << ")\n";
          rs.push(Pair(u, k));
          break;
        }
      }
    }
  }
  return rs;
}

std::vector<DFS::UserCall> DFS::ReverseDFS::simulate(std::stack<Pair> sj,
                                                     Pair until) {
  std::vector<UserCall> ret;
  std::cout << "sj empty: " << sj.empty() << "\n";
  while (!(sj.top() == until)) {
    Pair x = sj.top();
    sj.pop();
    uint u = x.head(), k = x.tail();
    if (c->get(u) == DFS_WHITE) {
      ret.push_back(UserCall(UserCall::Type::preprocess, u));
      c->insert(u, DFS_GRAY);
    }
    if (k < g->getNodeDegree(u)) {
      sj.push(Pair(u, k + 1));
      uint v = g->head(u, k);
      ret.push_back(UserCall(UserCall::Type::preexplore, u, v));
      if (c->get(v) == DFS_WHITE) {
        s->push(Pair(v, 0));
      } else {
        ret.push_back(UserCall(UserCall::Type::postexplore, u,
                               v));  // remove this later -> "missing parts"
      }
    } else {
      if (!sj.empty()) {
        uint pu = sj.top().head();
        ret.push_back(UserCall(UserCall::Type::postexplore, pu, u));
      }
      c->insert(u, DFS_BLACK);
      ret.push_back(UserCall(UserCall::Type::postprocess, u));
    }
  }
  sj.push(Pair(0, 0));
  ret.push_back(UserCall(UserCall::Type::preprocess, 42, 1));  // dummy
  return ret;
}

void DFS::ReverseDFS::storeTime(unsigned df, uint u) {
  // std::cout << "storeTime " << df << " " << u << "\n";
  if (ip >= r) throw std::out_of_range("too many intervals");
  Pair top = s->top();
  bool inserted = false;
  IntervalData &ci = i[ip];
  if (ci.ic == 0) {
    std::cout << "H" << ip << " = " << top.head() << "\n";
    ci.h1 = top;
  }
  if (df == 0) {
    if (d->get(u) == r + 1) {
      d->insert(u, ip);
      ns++;
      // std::cout << "d[" << u << "]=" << ip << "\n";
      inserted = true;
    }
  } else if (df == 1) {
    if (f->get(u) == r + 1) {
      f->insert(u, ip);
      ns--;
      // std::cout << "f[" << u << "]=" << ip << "\n";
      inserted = true;
    }
  } else {
    throw std::invalid_argument("storeTime: df must be 0 or 1");
  }
  if (inserted) {
    if (ci.hdc == -1 || ns < ci.hdc) {
      ci.hd = top;
      ci.hdc = ns;
    }
    if (ci.ic < w) {
      ci.ic++;
    } else {
      std::cout << "Ĥ" << ip << " = " << ci.hd.head() << "\n";
      ci.h2 = top;
      ip++;
    }
  }
}
void DFS::ReverseDFS::storeBeginTime(uint u) { storeTime(0, u); }
void DFS::ReverseDFS::storeEndTime(uint u) { storeTime(1, u); }
