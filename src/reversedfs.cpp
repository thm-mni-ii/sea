#include "sealib/reversedfs.h"
#include <math.h>
#include <iostream>

using namespace Sealib;  // NOLINT

void ReverseDFS::storeTime(unsigned df, uint u) {
  // std::cout << "storeTime " << df << " " << u << "\n";
  if (df == 0) {
    if (d.get(u) == r + 1) {
      d.insert(u, j);
      // std::cout << "d[" << u << "]=" << j << "\n";
    }
  } else if (df == 1) {
    if (f.get(u) == r + 1) {
      f.insert(u, j);
      // std::cout << "f[" << u << "]=" << j << "\n";
    }
  }
}

void ReverseDFS::updateInterval(bool end) {
  IntervalData &ci = i[j];
  Pair top = s.top();
  if (ci.size == 0) {
    ci.h1 = top;
  }
  ci.size++;
  if (ns < ci.hdc && ns > 0) {
    ci.hd = top;
    ci.hdc = ns;
  }
  if (ci.size == w + 1 || end) {
    ci.h2 = top;
    std::cout << "H" << j << " = (" << ci.h1.head() << "," << ci.h1.tail()
              << ")\n";
    std::cout << "Ĥ" << j << " = (" << ci.hd.head() << "," << ci.hd.tail()
              << ")\n";
    std::cout << "H'" << j << " = (" << ci.h2.head() << "," << ci.h2.tail()
              << ")\n";
    std::cout << "c" << j << " = " << ci.c1.type << ":" << ci.c1.u << ":"
              << ci.c1.v << "\n";
    j++;
    if (j >= r) throw std::out_of_range("too many intervals");
    updateInterval();
  }
}
void ReverseDFS::setCall(UserCall call) {
  if (call != UserCall() && i[j].c1 == UserCall()) {
    i[j].c1 = call;
  }
}

void ReverseDFS::process_recording(uint u0) {
  updateInterval();
  s.push(Pair(u0, 0));
  ns = 1;
  updateInterval();
  Pair x;
  while (!s.isEmpty()) {
    int sr = s.pop(&x);
    if (sr == DFS_DO_RESTORE) {
      restore_top(u0, g, &c, &s);
      s.pop(&x);
    } else if (sr == DFS_NO_MORE_NODES) {
      return;
    }
    uint u = x.head(), k = x.tail();
    if (c.get(u) == DFS_WHITE) {
      // preprocess(u);
      setCall({UserCall::preprocess, u});
      storeTime(0, u);
      c.insert(u, DFS_GRAY);
    }
    updateInterval();
    if (k < g->getNodeDegree(u)) {
      s.push(Pair(u, k + 1));
      updateInterval();
      uint v = g->head(u, k);
      // preexplore(u, v);
      setCall({UserCall::preexplore, u, v});
      if (c.get(v) == DFS_WHITE) {
        s.push(Pair(v, 0));
        ns++;
        updateInterval();
      } else {
        // postexplore(u, v);
        setCall({UserCall::postexplore, u, v});
      }
    } else {
      c.insert(u, DFS_BLACK);
      storeTime(1, u);
      // postprocess(u);
      setCall({UserCall::postprocess, u});
      if (u != u0) {
        Pair px;
        sr = s.pop(&px);
        if (sr == DFS_DO_RESTORE) {
          restore_top(u0, g, &c, &s);
          s.pop(&px);
        }
        uint pu = px.head();
        // postexplore(pu, u);
        setCall({UserCall::postexplore, pu, u});
        s.push(px);
      }
      ns--;
      updateInterval();
    }
  }
}

void ReverseDFS::init() {
  for (uint u = 0; u < n; u++) {
    if (c.get(u) == DFS_WHITE) process_recording(u);
  }
  updateInterval(true);
}

bool ReverseDFS::more() { return !(j == 0 && majorI == major.rend()); }

std::stack<Pair> ReverseDFS::reconstructPart(Pair from, Pair to) {
  std::cout << "reconstruct " << j << " " << from.head() << " " << to.head()
            << "\n";
  std::stack<Pair> rs;
  std::set<uint> tmp;
  if (to.head() != INVALID) {
    Pair a = Pair(from.head(), from.tail() - 1);
    do {
      uint u = a.head();
      std::cout << "u=" << u << "  ";
      if (u == to.head()) {
        rs.push(to);
      } else {
        bool found = false;
        for (uint k = a.tail(); k < g->getNodeDegree(u); k++) {
          uint v = g->head(u, k);
          std::cout << " -> " << v << "(c" << c.get(v) << ",d" << d.get(v)
                    << ",f" << f.get(v) << ")\n";
          if (f.get(v) == j &&
              tmp.find(v) == tmp.end()) {  // we need a fast way for recoloring;
                                           // until then: using a temp. set
            a = Pair(v, 0);
            rs.push(Pair(u, k + 1));
            tmp.insert(v);
            found = true;
            break;
          }
        }
        if (!found) {
          throw std::logic_error("no white path found in reconstruction");
        }
      }
    } while (rs.top() != to);
  } else {
    std::cout << "skipping reconstruction: H is invalid (bottom of stack)\n";
  }
  return rs;
}

std::vector<UserCall> ReverseDFS::simulate(std::stack<Pair> *sj, Pair until,
                                           UserCall first) {
  std::cout << "simulate " << j << " " << until.head() << "\n";
  std::vector<UserCall> ret;
  if (sj->empty() || first.u != sj->top().head()) {
    if (first.type == UserCall::preprocess) {
      sj->push(Pair(first.u, 0));
    } else if (first.type == UserCall::preexplore) {
      for (uint k = 0; k < g->getNodeDegree(first.u); k++) {
        if (g->head(first.u, k) == first.v) {
          sj->push(Pair(first.u, k));
          break;
        }
      }
    } else if (first.type == UserCall::postprocess) {
      ret.emplace_back(first);
      return ret;
    } else {
      throw std::logic_error("first.u != top, unknown type");
    }
  }
  while (sj->top() != until) {
    Pair x = sj->top();
    uint u = x.head(), k = x.tail();
    // std::cout << "top: " << u << "," << k << "\n";
    sj->pop();
    if (sj->size() > 0 && sj->top() == until) break;
    if (c.get(u) == DFS_WHITE) {
      // std::cout << "preproc " << u << "\n";
      ret.emplace_back(UserCall(UserCall::Type::preprocess, u));
      c.insert(u, DFS_GRAY);
    }
    if (k < g->getNodeDegree(u)) {
      sj->push(Pair(u, k + 1));
      if (sj->size() > 0 && sj->top() == until) break;
      uint v = g->head(u, k);
      if (c.get(v) == DFS_WHITE) {
        // std::cout << "preexp " << u << "," << v << "\n";
        ret.emplace_back(UserCall(UserCall::Type::preexplore, u, v));
        sj->push(Pair(v, 0));
        if (sj->size() > 0 && sj->top() == until) break;
      } else {
        // no postexplore: will be simulated later
        std::cout << " == pre+postexplore " << u << "," << v << " == \n";
      }
    } else {
      c.insert(u, DFS_BLACK);
      // std::cout << "postproc " << u << "\n";
      ret.emplace_back(UserCall(UserCall::Type::postprocess, u));
      if (sj->size() > 0) {
        uint pu = sj->top().head();
        // std::cout << "postexp " << pu << "," << u << "\n";
        ret.emplace_back(UserCall(UserCall::Type::postexplore, pu,
                                  u));  // this postexplore is necessary
      } else {
        bool a = false;
        for (uint b = u; b < n; b++) {
          if (c.get(b) == DFS_WHITE && d.get(b) == j) {
            sj->push(Pair(b, 0));
            a = true;
            break;
          }
        }
        if (!a) break;
      }
    }
  }
  return ret;
}

UserCall ReverseDFS::next() {
  if (majorI != major.rend()) {
    if (!waitStep) {
      UserCall &cc = *majorI;
      if (previous.u == cc.u && (cc.type == UserCall::preprocess ||
                                 cc.type == UserCall::postexplore)) {
        int k = static_cast<int>(g->getNodeDegree(cc.u)) - 1;
        if (previous.type == UserCall::preexplore) {
          while (k > 0 && g->head(cc.u, k) != previous.v) {
            k--;
          }
          k--;
        }
        while (k >= 0) {
          if (cc.type == UserCall::postexplore && g->head(cc.u, k) != cc.v)
            break;
          minor.emplace_back(
              UserCall(UserCall::preexplore, cc.u, g->head(cc.u, k)));
          minor.emplace_back(
              UserCall(UserCall::postexplore, cc.u, g->head(cc.u, k)));
          std::cout << " >>> minor: " << cc.u << "," << g->head(cc.u, k)
                    << "\n";
          k--;
        }
        if (minor.size() > 0) {
          waitStep = true;
        }
      }
    }
    if (minor.size() == 0) {
      previous = *majorI;
      waitStep = false;
      return *(majorI++);
    } else {
      UserCall m = minor.front();
      minor.pop_front();
      return m;
    }
  } else {  // build new sequence
    j--;
    for (uint a = 0; a < n; a++) {
      if (f.get(a) < j) {
        c.insert(a, DFS_BLACK);
      } else if (d.get(a) < j) {
        c.insert(a, DFS_GRAY);
      } else {
        c.insert(a, DFS_WHITE);
      }
    }
    std::stack<Pair> sj = reconstructPart(i[j].hd, i[j].h1);
    major = simulate(&sj, i[j].h2, i[j].c1);
    std::cout << "end of simulation " << j << "\n";
    majorI = major.rbegin();
    return next();
  }
}

ReverseDFS::ReverseDFS(Graph *graph)
    : g(graph),
      n(g->getOrder()),
      r(static_cast<uint>(ceil(8 * log2(n)))),
      w(static_cast<uint>(ceil(n / log2(n)))),
      c(n, 3),
      d(n, r + 1),
      f(n, r + 1),
      s(n, g, &c),
      i(new IntervalData[r]) {
  std::cout << "r=" << r << ", w=" << w << "\n";
  for (uint a = 0; a < n; a++) {
    d.insert(a, r + 1);
    f.insert(a, r + 1);
  }
}

ReverseDFS::~ReverseDFS() { delete[] i; }
