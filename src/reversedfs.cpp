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
      ns++;
      // std::cout << "d[" << u << "]=" << j << "\n";
    }
  } else if (df == 1) {
    if (f.get(u) == r + 1) {
      f.insert(u, j);
      ns--;
      // std::cout << "f[" << u << "]=" << j << "\n";
    }
  }
}

void ReverseDFS::updateInterval(Pair top, UserCall call, bool end) {
  IntervalData &ci = i[j];
  if (ci.h1.head() == INVALID && top.head() != INVALID) {
    std::cout << "H" << j << " = " << top.head() << "\n";
    ci.h1 = top;
  }
  if (ns < ci.hdc && top.head() != INVALID) {
    ci.hd = top;
    ci.hdc = ns;
  }
  if (ci.size < w && !end) {
    ci.size++;
  } else {
    if (end && ci.h1.head() == INVALID) {
      std::cout << "aborting invalid interval\n";
    } else {
      std::cout << "Ĥ" << j << " = " << ci.hd.head() << "\n";
      if (ci.h1.head() == INVALID || ci.hd.head() == INVALID) {
        throw std::logic_error("H/Ĥ is invalid");
      }
      ci.h2 = top;
      std::cout << "H'" << j << " = " << ci.h2.head() << "\n";
      j++;
    }
  }
}

std::stack<Pair> ReverseDFS::reconstructPart(Pair from, Pair to) {
  std::cout << "reconstruct " << j << " " << from.head() << " " << to.head()
            << "\n";
  std::stack<Pair> rs;
  if (from.head() != INVALID && to.head() != INVALID) {
    rs.push(from);
    std::cout << rs.top().head() << "," << rs.top().tail() << " = " << to.head()
              << "," << to.tail() << "?\n";
    while (!(rs.top().head() == to.head())) {  // only need to compare head()s
                                               // because the simulation will
                                               // sort it out
      Pair x = rs.top();
      uint u = x.head();
      std::cout << u << " ";
      bool found = false;
      for (uint k = 0; k < g->getNodeDegree(u); k++) {
        uint v = g->head(u, k);
        std::cout << " -> " << v << "(c" << c.get(v) << ",d" << d.get(v) << ",f"
                  << f.get(v) << ")\n";
        if (c.get(v) == DFS_WHITE && f.get(v) == j) {
          std::cout << "restoring (" << v << "," << 0 << ")\n";
          rs.push(Pair(v, 0));
          c.insert(v, DFS_GRAY);
          found = true;
          break;
        }
      }
      if (!found) {
        throw std::logic_error("no white path found in reconstruction");
      }
    }
  } else {
    throw std::logic_error("can not reconstruct invalid H/Ĥ");
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
    if (c.get(u) == DFS_WHITE) {
      std::cout << "preproc " << u << "\n";
      ret.emplace_back(UserCall(UserCall::Type::preprocess, u));
      c.insert(u, DFS_GRAY);
    }
    if (k < g->getNodeDegree(u)) {
      sj->push(Pair(u, k + 1));
      uint v = g->head(u, k);
      if (c.get(v) == DFS_WHITE) {
        std::cout << "preexp " << u << "," << v << "\n";
        ret.emplace_back(UserCall(UserCall::Type::preexplore, u, v));
        sj->push(Pair(v, 0));
      } else {
        // no postexplore: will be simulated later
      }
    } else {
      c.insert(u, DFS_BLACK);
      std::cout << "postproc " << u << "\n";
      ret.emplace_back(UserCall(UserCall::Type::postprocess, u));
      if (sj->size() > 0) {
        uint pu = sj->top().head();
        std::cout << "postexp " << pu << "," << u << "\n";
        ret.emplace_back(UserCall(UserCall::Type::postexplore, pu,
                                  u));  // this postexplore is necessary
      } else {
        bool a = false;
        for (; u < n; u++) {
          if (c.get(u) == DFS_WHITE && d.get(u) == j) {
            sj->push(Pair(u, 0));
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

void ReverseDFS::init() {
  ExtendedSegmentStack s(n, g, &c);  // used for recording run
  for (uint a = 0; a < n; a++) c.insert(a, DFS_WHITE);
  bool skipNext = false;
  for (uint a = 0; a < n; a++) {
    if (c.get(a) == DFS_WHITE) {
      DFS::process_small(
          a, g, &c, &s, DFS::restore_top,
          [this, &s](uint u) {
            storeTime(0, u);
            updateInterval(s.top(), UserCall(UserCall::preprocess, u));
          },
          [this, &s, &skipNext](uint u, uint v) {
            if (c.get(v) == DFS_WHITE) {  // ?
              updateInterval(s.top(), UserCall(UserCall::preexplore, u, v));
            } else {
              skipNext = true;
            }
          },
          [this, &s, &skipNext](uint u, uint v) {
            if (!skipNext) {
              updateInterval(s.top(), UserCall(UserCall::postexplore, u, v));
            } else {
              skipNext = false;
            }
          },
          [this, &s](uint u) {
            storeTime(1, u);
            updateInterval(s.top(), UserCall(UserCall::postprocess, u));
          });
    }
  }
  updateInterval(s.top(), UserCall(UserCall::postprocess, 0), true);
}

bool ReverseDFS::more() { return j != 0 || sp != seq.size() - 1; }

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
      i(new IntervalData[r]) {
  std::cout << "r=" << r << ", w=" << w << "\n";
  for (uint a = 0; a < n; a++) {
    d.insert(a, r + 1);
    f.insert(a, r + 1);
  }
}

ReverseDFS::~ReverseDFS() { delete[] i; }
