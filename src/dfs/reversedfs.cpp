#include "sealib/iterator/reversedfs.h"
#include <cmath>
#include <set>

namespace Sealib {

ReverseDFS::ReverseDFS(Graph const &graph)
    : g(graph),
      n(g.getOrder()),
      iCount(static_cast<uint64_t>(4 * log2(n) / log2(log2(n))) + 1),
      iWidth(static_cast<uint64_t>(n * log2(log2(n)) / log2(n)) + 1),
      c(n, 3),
      d(n, iCount + 1),
      f(n, iCount + 1),
      s(n, g, &c),
      intervals(1),
      i(intervals.begin()),
      sequence(),
      seqI(sequence.rend()) {
    for (uint64_t a = 0; a < n; a++) {
        d.insert(a, iCount);
    }
}

const std::pair<uint64_t, uint64_t> ReverseDFS::NIL = {INVALID, INVALID};

void ReverseDFS::init() {
    i->firstCall = UserCall(UserCall::preprocess, 0);
    UserCall::Type trace;
    for (uint64_t u0 = 0; u0 < n; u0++) {
        if (c.get(u0) == DFS_WHITE) {
            DFS::visit_nloglogn(
                u0, g, &c, &s, DFS::restore_top,
                [this, &trace](uint64_t u) {
                    if (i->width == iWidth) {
                        nextInterval();
                        if (i->depth > 0) needBelowTop.push_back(ip);
                        i->firstCall = UserCall(UserCall::preprocess, u);
                    }
                    d.insert(u, ip);
                    i->width++;
                    trace = UserCall::preprocess;
                },
                [this, &trace](uint64_t u, uint64_t k) {
                    if (needTopOfStack) {
                        needTopOfStack = false;
                        // NOT (u,k+1): we need the top entry at time of a
                        // previous call
                        i->top = {u, k};
                        if (i->bottom == NIL) i->bottom = {u, k};
                    }
                    uint64_t v = g.head(u, k);
                    if (c.get(v) == DFS_WHITE) {
                        // major preexplore
                        if (i->width == iWidth) {
                            nextInterval();
                            i->top = i->bottom = {u, k + 1};
                            i->firstCall = UserCall(UserCall::preexplore, u, k);
                        }
                        i->width++;
                    }
                    trace = UserCall::preexplore;
                },
                [this, &trace](uint64_t u, uint64_t k) {
                    if (trace != UserCall::preexplore) {
                        // major postexplore
                        if (!needBelowTop.empty()) {
                            uint64_t b = needBelowTop.back();
                            if (intervals[b].depth - 1 == s.size()) {
                                needBelowTop.pop_back();
                                intervals[b].top = {u, k + 1};
                                intervals[b].bottom = {u, k + 1};
                            }
                        }
                        if (needTopOfStack) {
                            // we have cycled back into the postexplore
                            needTopOfStack = false;
                            uint64_t v = g.head(u, k);
                            i->top = {v, g.deg(v)};
                            i->bottom = {v, g.deg(v)};
                        }
                        if (i->width == iWidth) {
                            nextInterval();
                            if (i->depth > 0) needTopOfStack = true;
                            i->firstCall =
                                UserCall(UserCall::postexplore, u, k);
                        } else if (s.size() < i->depth) {
                            // track depth
                            i->depth = s.size();
                            i->bottom = {u, k + 1};
                        }
                        i->width++;
                    }
                    trace = UserCall::postexplore;
                },
                [this, &trace](uint64_t u) {
                    if (i->width == iWidth) {
                        nextInterval();
                        if (i->depth > 0) needTopOfStack = true;
                        i->firstCall = UserCall(UserCall::postprocess, u);
                    }
                    f.insert(u, ip);
                    i->width++;
                    trace = UserCall::postprocess;
                });
        }
    }
    assert(ip < iCount);
}

void ReverseDFS::nextInterval() {
    intervals.emplace_back(IntervalData());
    ip++;
    i = intervals.begin();
    std::advance(i, static_cast<int64_t>(ip));
    i->depth = s.size();
}

bool ReverseDFS::more() {
    return static_cast<int64_t>(ip) != -1 || seqI != sequence.rend();
}

UserCall ReverseDFS::next() {
    if (haveNext) {
        return insertMinor();
    }
    if (seqI != sequence.rend()) {
        UserCall r = *seqI;
        if (latestOutput.type == UserCall::preexplore && latestOutput.k > 0 &&
            r.type == UserCall::preprocess) {
            insertLast = {r.u, 0};
            insertNext = {r.u, latestOutput.k - 1};
            haveNext = true;
            nextType = UserCall::postexplore;
        } else if (latestOutput.type == UserCall::postprocess &&
                   r.type == UserCall::preprocess && g.deg(r.u) > 0) {
            insertLast = {r.u, 0};
            insertNext = {r.u, g.deg(r.u) - 1};
            haveNext = true;
            nextType = UserCall::postexplore;
        } else if (latestOutput.type == UserCall::postprocess &&
                   r.type == UserCall::postexplore && r.k != g.deg(r.u) - 1) {
            insertLast = {r.u, r.k + 1};
            insertNext = {r.u, g.deg(r.u) - 1};
            haveNext = true;
            nextType = UserCall::postexplore;
        } else if (latestOutput.type == UserCall::preexplore &&
                   r.type == UserCall::postexplore &&
                   latestOutput.k > r.k + 1) {
            insertLast = {r.u, r.k + 1};
            insertNext = {r.u, latestOutput.k - 1};
            haveNext = true;
            nextType = UserCall::postexplore;
        }
        if (haveNext) {
            return insertMinor();
        } else {
            latestOutput = r;
            std::advance(seqI, 1);
            return r;
        }
    } else {  // build new sequence
        for (uint64_t a = 0; a < n; a++) {
            if (f.get(a) < ip) {
                c.insert(a, DFS_BLACK);
            } else if (d.get(a) < ip) {
                c.insert(a, DFS_GRAY);
            } else {
                c.insert(a, DFS_WHITE);
            }
        }
        std::stack<std::pair<uint64_t, uint64_t>> sj = reconstructStack();
        sequence = simulate(&sj);
        seqI = sequence.rbegin();
        std::advance(i, -1);
        ip--;
        return next();
    }
}

UserCall ReverseDFS::insertMinor() {
    UserCall r(nextType, insertNext.first, insertNext.second);
    if (nextType == UserCall::preexplore) {
        if (insertNext == insertLast) {
            haveNext = false;
        } else {
            insertNext.second--;
            nextType = UserCall::postexplore;
        }
    } else if (nextType == UserCall::postexplore) {
        nextType = UserCall::preexplore;
    }
    latestOutput = r;
    return r;
}

std::stack<std::pair<uint64_t, uint64_t>> ReverseDFS::reconstructStack() {
    std::stack<std::pair<uint64_t, uint64_t>> r;
    std::vector<bool> used(n, 0);
    if (i->bottom != NIL) {
        // restore all u with d[u]<j and f[u]==j
        std::pair<uint64_t, uint64_t> a = i->bottom;
        r.push(a);
        used[a.first] = true;
        while (a.first != i->top.first) {
            for (uint64_t b = 0; b < g.deg(a.first); b++) {
                uint64_t v = g.head(a.first, b);
                if (d.get(v) < ip && f.get(v) == ip && !used[v]) {
                    used[v] = true;
                    r.top().second = b + 1;
                    a = {v, INVALID};
                    break;
                }
            }
            if (a.second == INVALID)
                r.push(a);
            else
                throw StackReconstructionFailed();
        }  // loop only ends when top has been pushed
        r.top() = i->top;
    }
    return r;
}

std::vector<UserCall> ReverseDFS::simulate(
    std::stack<std::pair<uint64_t, uint64_t>> *sj) {
    std::vector<UserCall> r;
    if (i->firstCall.type == UserCall::preprocess) {
        sj->push({i->firstCall.u, 0});
    } else if (i->firstCall.type == UserCall::preexplore) {
        // we don't have to be careful here because u must be gray
        sj->top().second -= 1;
    } else if (i->firstCall.type == UserCall::postprocess) {
        if (sj->empty()) sj->push({i->firstCall.u, g.deg(i->firstCall.u)});
    } else if (i->firstCall.type == UserCall::postexplore) {
        r.emplace_back(
            UserCall(UserCall::postexplore, i->firstCall.u, i->firstCall.k));
        if (sj->empty()) sj->push({i->firstCall.u, i->firstCall.k + 1});
        // now, proceed normally
    }

    do {
        while (!sj->empty() && r.size() < i->width) {
            std::pair<uint64_t, uint64_t> x = sj->top();
            sj->pop();
            uint64_t u = x.first, k = x.second;
            if (c.get(u) == DFS_WHITE) {
                r.emplace_back(UserCall(UserCall::preprocess, u));
                c.insert(u, DFS_GRAY);
            }
            if (k < g.deg(u) && r.size() < i->width) {
                sj->push({u, k + 1});
                uint64_t v = g.head(u, k);
                if (c.get(v) == DFS_WHITE && r.size() < i->width) {
                    r.emplace_back(UserCall(UserCall::preexplore, u, k));
                    sj->push({v, 0});
                }
            } else if (r.size() < i->width) {
                c.insert(u, DFS_BLACK);
                r.emplace_back(UserCall(UserCall::Type::postprocess, u));
                if (!sj->empty() && r.size() < i->width) {
                    std::pair<uint64_t, uint64_t> p = sj->top();
                    r.emplace_back(UserCall(UserCall::Type::postexplore,
                                            p.first, p.second - 1));
                }
            }
        }
        if (sj->empty() && r.size() < i->width) {
            for (uint64_t u = 0; u < n; u++) {
                if (c.get(u) == DFS_WHITE) {
                    sj->push({u, 0});
                    break;
                }
            }
        }
    } while (!sj->empty() && r.size() < i->width);
    return r;
}

}  // namespace Sealib
