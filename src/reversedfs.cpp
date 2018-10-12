#include "sealib/reversedfs.h"
#include <math.h>
#include <iostream>

using Sealib::Pair;
using Sealib::DFS;
using Sealib::ReverseDFS;
using Sealib::UserCall;

void ReverseDFS::storeTime(unsigned df, uint u) {
  // std::cout << "storeTime " << df << " " << u << "\n";
  if (j >= r) throw std::out_of_range("too many intervals");
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

void ReverseDFS::updateInterval(/*UserCall call, */ bool end) {
  IntervalData &ci = i[j];
  Pair top = s.top();
  if (ns > 0 && ci.size <= 1) {
    ci.h1 = top;
  }
  if (ns < ci.hdc && ns > 0) {
    ci.hd = top;
    ci.hdc = ns;
  }
  if (ci.size < w && !end) {
    ci.size++;
  } else {
    ci.h2 = top;
    std::cout << "H" << j << " = " << ci.h1.head() << "\n";
    std::cout << "Ĥ" << j << " = " << ci.hd.head() << "\n";
    std::cout << "H'" << j << " = " << ci.h2.head() << "\n";
    j++;
  }
}

void ReverseDFS::process_recording(uint u0) {
  updateInterval();
  s.push(Pair(u0, 0));
  ns = 1;
  Pair x;
  while (!s.isEmpty()) {
    updateInterval();
    int sr = s.pop(&x);
    ns--;
    if (sr == DFS_DO_RESTORE) {
      restore_top(u0, g, &c, &s);
      s.pop(&x);
    } else if (sr == DFS_NO_MORE_NODES) {
      return;
    }
    uint u = x.head(), k = x.tail();
    if (c.get(u) == DFS_WHITE) {
      // preprocess(u);
      storeTime(0, u);
      c.insert(u, DFS_GRAY);
    }
    if (k < g->getNodeDegree(u)) {
      updateInterval();
      s.push(Pair(u, k + 1));
      ns++;
      uint v = g->head(u, k);
      // preexplore(u, v);
      if (c.get(v) == DFS_WHITE) {
        updateInterval();
        s.push(Pair(v, 0));
        ns++;
      } else {
        // postexplore(u, v);
      }
    } else {
      c.insert(u, DFS_BLACK);
      storeTime(1, u);
      // postprocess(u);
      if (u != u0) {
        Pair px;
        sr = s.pop(&px);
        ns--;
        if (sr == DFS_DO_RESTORE) {
          restore_top(u0, g, &c, &s);
          s.pop(&px);
        }
        uint pu = px.head();
        // postexplore(pu, u);
        updateInterval();
        s.push(px);
        ns++;
      }
    }
  }
}

void ReverseDFS::init() {
  for (uint u = 0; u < n; u++) {
    if (c.get(u) == DFS_WHITE) process_recording(u);
  }
  updateInterval(true);
}

bool ReverseDFS::more() { return !(j == 0 && sp == seq.size() - 1); }

std::stack<Pair> ReverseDFS::reconstructPart(Pair from, Pair to) {
  std::cout << "reconstruct " << j << " " << from.head() << " " << to.head()
            << "\n";
  std::stack<Pair> rs;
  if (from.head() != INVALID && to.head() != INVALID) {
    if (from != to) {
      rs.push(Pair(from.head(),from.tail()-1));
      while (rs.top() != to) {
        Pair x = rs.top();
        rs.pop();
        uint u = x.head(), k = x.tail();
        if (k < g->getNodeDegree(u)) {
          rs.push(Pair(u, k + 1));
          uint v = g->head(u, k);
          std::cout << u << " -> " << v << "(c" << c.get(v) << ",d" << d.get(v)
                    << ",f" << f.get(v) << ")\n";
          if (c.get(v) == DFS_WHITE) {
            std::cout << "restoring (" << v << "," << 0 << ")\n";
            rs.push(Pair(v, 0));
            c.insert(v, DFS_GRAY);
          }
        } else {
          throw std::logic_error("no white path found in reconstruction");
        }
      }
    } else {
      rs.push(from);
    }
    /*uint u = x.head();
    std::cout << u << " ";
    bool found = false;
    for (uint k = 0; k < g->getNodeDegree(u); k++) {
      uint v = g->head(u, k);
      std::cout << " -> " << v << "(c" << c.get(v) << ",d" << d.get(v) << ",f"
                << f.get(v) << ")\n";
      if (c.get(v) == DFS_WHITE && f.get(v) == j) {
        uint vk;
        for(vk=0; vk<g->getNodeDegree(v); vk++) {
          if(c.get(g->head(v,vk))&&f.get(g->head(v,vk))==j) break;
        }
        std::cout << "restoring (" << v << "," << vk << ")\n";
        rs.push(Pair(v, vk));
        c.insert(v, DFS_GRAY);
        found = true;
        break;
      }
    }
    if (!found) {
      throw std::logic_error("no white path found in reconstruction");
    }
  }*/
  } else {
    throw std::logic_error("reconstruction: H/Ĥ is invalid (bottom of stack)");
  }
  return rs;
}

std::vector<UserCall> ReverseDFS::simulate(std::stack<Pair> *sj, Pair until) {
  std::cout << "simulate " << sj->top().head() << " " << until.head() << "\n";
  std::vector<UserCall> ret;
  while (sj->top() != until) {
    Pair x = sj->top();
    uint u = x.head(), k = x.tail();
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
      }
    } else {
      c.insert(u, DFS_BLACK);
      std::cout << "postproc " << u << "\n";
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
  if (sp < seq.size()) {
    sp++;
    return seq.at(seq.size() - sp);
  } else {  // build new sequence
    j--;
    for (uint a = 0; a < n; a++) {
      if (f.get(a) < j) {
        c.insert(a, DFS_BLACK);
      } else if (d.get(a) < j) {
        c.insert(a, DFS_GRAY);
      } else {
        c.insert(a, DFS_WHITE);
        std::cout << a << " ";
      }
    }
    std::cout<<"\n";
    for (uint a = 0; a < n; a++) {
      if (c.get(a) == DFS_GRAY && f.get(a) == j) {
        c.insert(a, DFS_WHITE);
      }
    }
    std::stack<Pair> sj = reconstructPart(i[j].hd, i[j].h1);
    seq = simulate(&sj, i[j].h2);
    std::cout << "end of simulation " << j << "\n";
    sp = 1;
    return seq.at(seq.size() - 1);
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
