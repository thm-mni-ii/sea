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
                             UserFunc1 preprocess, UserFunc2 preexplore,
                             UserFunc2 postexplore, UserFunc1 postprocess) {
  std::stack<Pair> *s = new std::stack<Pair>;
  s->push(Pair(u0, 0));
  while (!s->empty()) {
    Pair x = s->top();
    s->pop();
    uint u = x.head();
    uint k = x.tail();
    if (color[u] == DFS_WHITE) {
      preprocess(u);
      color[u] = DFS_GRAY;
    }
    if (k < g->getNodeDegree(u)) {
      s->push(Pair(u, k + 1));
      uint v = g->head(u, k);
      preexplore(u, v);
      if (color[v] == DFS_WHITE) {
        s->push(Pair(v, 0));
      } else {
        postexplore(u, v);
      }
    } else {
      if (u != u0) {
        uint pu = s->top().head();
        postexplore(pu, u);
      }
      color[u] = DFS_BLACK;
      postprocess(u);
    }
  }
  delete s;
}

template <class SS>
static void process_small(uint u0, Graph *g, CompactArray *color, SS *s,
                          void (*restoration)(uint, Graph *, CompactArray *,
                                              SS *),
                          UserFunc1 preprocess, UserFunc2 preexplore,
                          UserFunc2 postexplore, UserFunc1 postprocess) {
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
      preprocess(u);
      color->insert(u, DFS_GRAY);
    }
    if (k < g->getNodeDegree(u)) {
      s->push(Pair(u, k + 1));
      uint v = g->head(u, k);
      preexplore(u, v);
      if (color->get(v) == DFS_WHITE) {
        s->push(Pair(v, 0));
      } else {
        postexplore(u, v);
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
        postexplore(pu, u);
        s->push(px);
      }
      color->insert(u, DFS_BLACK);
      postprocess(u);
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

void DFS::standardDFS(Graph *g, UserFunc1 preprocess, UserFunc2 preexplore,
                      UserFunc2 postexplore, UserFunc1 postprocess) {
  uint *color = new uint[g->getOrder()];
  for (uint a = 0; a < g->getOrder(); a++) color[a] = DFS_WHITE;
  for (uint u = 0; u < g->getOrder(); u++) {
    if (color[u] == DFS_WHITE) {
      process_standard(u, g, color, preprocess, preexplore, postexplore,
                       postprocess);
    }
  }
  delete[] color;
}

void DFS::nBitDFS(Graph *g, UserFunc1 preprocess, UserFunc2 preexplore,
                  UserFunc2 postexplore, UserFunc1 postprocess) {
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
      process_small(a, g, &color, &s, restore_full, preprocess, preexplore,
                    postexplore, postprocess);
  }
}

void DFS::nloglognBitDFS(Graph *g, UserFunc1 preprocess, UserFunc2 preexplore,
                         UserFunc2 postexplore, UserFunc1 postprocess) {
  unsigned int n = g->getOrder();
  CompactArray color(n, 3);
  ExtendedSegmentStack s(n, g, &color);
  for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
  for (uint a = 0; a < n; a++) {
    if (color.get(a) == DFS_WHITE)
      process_small(a, g, &color, &s, restore_top, preprocess, preexplore,
                    postexplore, postprocess);
  }
}

void DFS::runLinearTimeInplaceDFS(uint *graph, UserFunc1 preprocess,
                                  UserFunc1 postprocess, uint startVertex) {
  auto *ilDFSRunner =
      new LinearTimeInplaceDFSRunner(graph, preprocess, postprocess);
  ilDFSRunner->run(startVertex);
  delete ilDFSRunner;
}

// reverse DFS:

void DFS::ReverseDFS::storeTime(unsigned df, uint u) {
  // std::cout << "storeTime " << df << " " << u << "\n";
  if (ip >= r) throw std::out_of_range("too many intervals");
  if (df == 0) {
    if (d->get(u) == r + 1) {
      d->insert(u, ip);
      ns++;
      // std::cout << "d[" << u << "]=" << ip << "\n";
    }
  } else if (df == 1) {
    if (f->get(u) == r + 1) {
      f->insert(u, ip);
      ns--;
      // std::cout << "f[" << u << "]=" << ip << "\n";
    }
  }
}

void DFS::ReverseDFS::updateInterval(Pair top, bool end) {
  IntervalData &ci = i[ip];
  if (ci.h1.head() == INVALID && top.head() != INVALID) {
    std::cout << "H" << ip << " = " << top.head() << "\n";
    ci.h1 = top;
  }
  if (ns < ci.hdc && top.head() != INVALID) {
    ci.hd = top;
    ci.hdc = ns;
  }
  if (ci.size < w && !end) {
    ci.size++;
  } else {
    if (end) std::cout << "ending interval " << ip << "\n";
    std::cout << "Ĥ" << ip << " = " << ci.hd.head() << "\n";
    if (ci.h1.head() == INVALID || ci.hd.head() == INVALID) {
      throw std::logic_error("H/Ĥ is invalid");
    }
    ci.h2 = top;
    std::cout << "H'" << ip << " = " << ci.h2.head() << "\n";
    ip++;
  }
}

std::stack<Pair> DFS::ReverseDFS::reconstructPart(uint j, Pair from, Pair to) {
  std::cout << "reconstruct " << j << " " << from.head() << " " << to.head()
            << "\n";
  std::stack<Pair> rs;
  if (from.head() != INVALID && to.head() != INVALID) {
    rs.push(from);
    std::cout << rs.top().head() << " = " << to.head() << "?\n";
    while (!(rs.top() == to)) {
      Pair x = rs.top();
      uint u = g->head(x.head(), x.tail());
      for (uint k = 0; k < g->getNodeDegree(u); k++) {
        uint v = g->head(u, k);
        if (c->get(v) == DFS_WHITE) {
          std::cout << "restoring (" << u << "," << k << ")\n";
          rs.push(Pair(u, k));
          break;
        }
      }
    }
  } else {
    throw std::logic_error("can not reconstruct invalid H/Ĥ");
  }
  return rs;
}

std::vector<DFS::UserCall> DFS::ReverseDFS::simulate(std::stack<Pair> sj,
                                                     Pair until) {
  std::cout << "simulate " << sj.top().head() << " " << until.head() << "\n";
  std::vector<UserCall> ret;
  Pair x;
  while (sj.top() != until) {
    bool a = false;
    if (until.head() == INVALID && sj.empty()) {
      for (uint u = x.head(); u < n; u++) {
        if (c->get(u) == DFS_WHITE) {
          sj.push(Pair(u, 0));
          a = true;
          break;
        }
      }
      if (!a) break;
    }
    x = sj.top();
    sj.pop();
    uint u = x.head(), k = x.tail();
    if (c->get(u) == DFS_WHITE) {
      std::cout << "preproc " << u << "\n";
      ret.emplace_back(UserCall(UserCall::Type::preprocess, u));
      c->insert(u, DFS_GRAY);
    }
    if (k < g->getNodeDegree(u)) {
      sj.push(Pair(u, k + 1));
      uint v = g->head(u, k);
      if (c->get(v) == DFS_WHITE) {
        std::cout << "preexp " << u << "," << v << "\n";
        ret.emplace_back(UserCall(UserCall::Type::preexplore, u, v));
        sj.push(Pair(v, 0));
      } else {
        // ret.emplace_back(UserCall(UserCall::Type::postexplore, u, v));  //->
        // "missing parts"
      }
    } else {
      if (!sj.empty()) {
        uint pu = sj.top().head();
        std::cout << "postexp " << pu << "," << u << "\n";
        ret.emplace_back(UserCall(UserCall::Type::postexplore, pu, u));
      }
      c->insert(u, DFS_BLACK);
      std::cout << "postproc " << u << "\n";
      ret.emplace_back(UserCall(UserCall::Type::postprocess, u));
    }
  }
  return ret;
}

void DFS::ReverseDFS::init() {
  ExtendedSegmentStack s(n, g, c);  // used for recording run
  for (uint a = 0; a < n; a++) c->insert(a, DFS_WHITE);
  for (uint a = 0; a < n; a++) {
    if (c->get(a) == DFS_WHITE) {
      process_small(a, g, c, &s, restore_top,
                    [this, &s](uint u) {
                      storeTime(0, u);
                      updateInterval(s.top());
                    },
                    [this, &s](uint u, uint v) {
                      u = v;
                      updateInterval(s.top());
                    },
                    [this, &s](uint u, uint v) {
                      u = v;
                      updateInterval(s.top());
                    },
                    [this, &s](uint u) {
                      storeTime(1, u);
                      updateInterval(s.top());
                    });
    }
  }
  updateInterval(s.top(), true);
}

bool DFS::ReverseDFS::more() { return static_cast<int>(ip) >= 0; }

DFS::UserCall DFS::ReverseDFS::next() {
  if (sp < seq.size()) {
    sp++;
    return seq.at(seq.size() - sp);
  } else {  // build new sequence
    ip--;
    for (uint a = 0; a < n; a++) {
      if (f->get(a) < ip)
        c->insert(a, DFS_BLACK);
      else if (d->get(a) < ip)
        c->insert(a, DFS_GRAY);
      else
        c->insert(a, DFS_WHITE);
    }
    std::stack<Pair> sj = reconstructPart(ip, i[ip].hd, i[ip].h1);
    seq = simulate(sj, i[ip].h2);
    sp = 1;
    return seq.at(seq.size() - 1);
  }
}

DFS::ReverseDFS::ReverseDFS(Graph *graph)
    : g(graph),
      n(g->getOrder()),
      r(static_cast<uint>(ceil(8 * log2(n)))),
      w(static_cast<uint>(ceil(n / log2(n)))),
      c(new CompactArray(n, 3)),
      d(new CompactArray(n, r + 1)),
      f(new CompactArray(n, r + 1)),
      i(new IntervalData[r]) {
  std::cout << "r=" << r << ", w=" << w << "\n";
  for (uint a = 0; a < n; a++) {
    d->insert(a, r + 1);
    f->insert(a, r + 1);
  }
}

DFS::ReverseDFS::~ReverseDFS() {
  delete c;
  delete d;
  delete f;
  delete[] i;
}
