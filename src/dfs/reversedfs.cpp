#include "sealib/iterator/reversedfs.h"
#include <cmath>
#include <set>

namespace Sealib {

ReverseDFS::ReverseDFS(Graph const &graph)
    : g(graph),
      n(g.getOrder()),
      iCount(static_cast<uint64_t>(20 * log2(n) / log2(log2(n))) + 1),
      iWidth(static_cast<uint64_t>(n * log2(log2(n)) / log2(n)) + 1),
      c(n, 3),
      d(n, iCount + 1),
      f(n, iCount + 1),
      s(n, g, &c),
      intervals(iCount),
      i(intervals.begin()),
      sequence(),
      seqI(sequence.rend()) {
    for (uint64_t a = 0; a < n; a++) {
        c.insert(a, 0);
        d.insert(a, iCount);
        f.insert(a, iCount);
    }
}

void ReverseDFS::init() {
    i->top = i->bottom = {INVALID, INVALID};
    i->firstCall = UserCall(UserCall::preprocess, 0);
    UserCall::Type trace;
    for (uint64_t u0 = 0; u0 < n; u0++) {
        if (c.get(u0) == DFS_WHITE) {
            DFS::visit_nloglogn(
                u0, g, &c, &s, DFS::restore_top,
                [this, &trace, &u0](uint64_t u) {
                    if (i->width > iWidth) {
                        nextInterval();
                        if (u != u0)
                            needBelowTop = true;
                        else
                            i->top = i->bottom = {INVALID, INVALID};
                        i->firstCall = UserCall(UserCall::preprocess, u);
                    }
                    d.insert(u, ip);
                    i->width++;
                    trace = UserCall::preprocess;
                },
                [this, &trace](uint64_t u, uint64_t k) {
                    if (needTopOfStack) {
                        needTopOfStack = false;
                        // NOT (u,k+1): we need the top entry from a previous
                        // call
                        i->top = i->bottom = {u, k};
                    }
                    uint64_t v = g.head(u, k);
                    if (d.get(v) == iCount + 1) {
                        if (i->width > iWidth) {
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
                        if (needBelowTop) {
                            needBelowTop = false;
                            i->top = i->bottom = {u, k + 1};
                        }
                        if (i->width > iWidth) {
                            nextInterval();
                            needTopOfStack = true;
                            i->firstCall =
                                UserCall(UserCall::postexplore, u, k);
                        } else if (s.size() < i->depth && s.size() > 0) {
                            i->depth = s.size();
                            i->bottom = {u, k + 1};
                        }
                        i->width++;
                    }
                    trace = UserCall::postexplore;
                },
                [this, &trace](uint64_t u) {
                    if (i->width > iWidth) {
                        needTopOfStack = true;
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
    i++;
    ip++;
    i->depth = s.size();
}

void ReverseDFS::trackSize(uint64_t u0) {
    if (s.size() < i->depth) {
        i->depth = s.size();
        if (i->depth > 0) {
            i->bottom = s.top();  // may be INVALID, restore?
            if (i->bottom.first == INVALID) {
                DFS::restore_top(u0, g, &c, &s);
                i->bottom = s.top();
            }
        } else {
            i->bottom = {INVALID, INVALID};
        }
    }
}

UserCall ReverseDFS::next() {
    if (seqI != sequence.rend()) {
        UserCall r = *seqI;
        seqI++;
        return r;
        /*static uint64_t mu = INVALID, mk = 0;
        static bool delayedPostexp = false;
        UserCall &cc = *seqI;
        UserCall ret;
        if (delayedPostexp) {
            delayedPostexp = false;
            ret = UserCall(UserCall::postexplore, cc.u, g.head(cc.u, mk));
        } else if (previous.u == cc.u && (cc.type == UserCall::preprocess ||
                                          cc.type == UserCall::postexplore)) {
            if (mu == INVALID) {
                mk = previous.k - 1;
            } else {
                mk--;
            }
            if (static_cast<int>(mk) >= 0) {
                uint64_t v = g.head(cc.u, mk);
                delayedPostexp = true;
                ret = UserCall(UserCall::preexplore, cc.u, v);
            } else {
                mu = INVALID;
            }
        }
        if (mu == INVALID) {
            previous = *seqI;
            ret = *(seqI++);
        }
        return ret;*/
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
        i--;
        ip--;
        return next();
    }
}

std::stack<std::pair<uint64_t, uint64_t>> ReverseDFS::reconstructStack() {
    std::stack<std::pair<uint64_t, uint64_t>> r;
    std::vector<bool> used(n, 0);
    if (i->bottom.first != INVALID) {
        // restore all u with d[u]<j and f[u]==j
        std::pair<uint64_t, uint64_t> a = i->bottom;
        r.push(a);
        while (a != i->top) {
            for (uint64_t b = 0; b < g.deg(a.first); b++) {
                uint64_t v = g.head(a.first, b);
                if (f.get(v) == ip && d.get(v) < ip && !used[v]) {
                    used[v] = true;
                    r.top().second = b + 1;
                    a = {v, INVALID};
                    break;
                }
            }
            if (a.second == INVALID)
                r.push(a);
            else
                throw NodeReconstructionFailed();
        }  // loop only ends when top has been pushed
        r.top() = i->top;
    }
    return r;
}

std::vector<UserCall> ReverseDFS::simulate(
    std::stack<std::pair<uint64_t, uint64_t>> *sj) {
    std::vector<UserCall> r;
    std::pair<uint64_t, uint64_t> end = (i + 1)->top;
    if (sj->empty()) {
        throw IntervalStackEmpty();
    }
    if (i->firstCall.type == UserCall::postprocess) {
        r.emplace_back(UserCall(UserCall::postprocess, i->firstCall.u));
    }
    if (i->firstCall.type == UserCall::postexplore) {
        r.emplace_back(
            UserCall(UserCall::postexplore, i->firstCall.u, i->firstCall.k));
        // now, proceed normally
    }

    do {
        while (!sj->empty() && sj->top() != end) {
            std::pair<uint64_t, uint64_t> x = sj->top();
            sj->pop();
            uint64_t u = x.first, k = x.second;
            if (c.get(u) == DFS_WHITE) {
                r.emplace_back(UserCall(UserCall::preprocess, u));
                c.insert(u, DFS_GRAY);
            }
            if (k < g.deg(u)) {
                sj->push({u, k + 1});
                uint64_t v = g.head(u, k);
                if (c.get(v) == DFS_WHITE) {
                    r.emplace_back(UserCall(UserCall::preexplore, u, k));
                    sj->push({v, 0});
                }
            } else {
                c.insert(u, DFS_BLACK);
                r.emplace_back(UserCall(UserCall::Type::postprocess, u));
                if (sj->size() > 0) {
                    std::pair<uint64_t, uint64_t> p = sj->top();
                    r.emplace_back(UserCall(UserCall::Type::postexplore,
                                            p.first, p.second - 1));
                }
            }
        }
        if (sj->empty()) {
            for (uint64_t u = 0; u < n; u++) {
                if (c.get(u) == DFS_WHITE) {
                    sj->push({u, 0});
                    break;
                }
            }
        }
    } while (!sj->empty());
    return r;
}

/*std::vector<UserCall> ReverseDFS::simulate(
    std::stack<std::pair<uint64_t, uint64_t>> *const sj,
    std::pair<uint64_t, uint64_t> until, UserCall first) {
    std::vector<UserCall> ret;

    if (sj->empty()) {
        if (first.type == UserCall::preprocess) {
            sj->push(std::pair<uint64_t, uint64_t>(first.u, 0));
        } else if (first.type == UserCall::preexplore) {
            sj->push(std::pair<uint64_t, uint64_t>(first.u, first.k));
        } else {
            throw std::logic_error("stack empty");
        }
    }

    while (sj->top() != until) {
        std::pair<uint64_t, uint64_t> x = sj->top();
        sj->pop();
        uint64_t u = x.first, k = x.second;
        if (k == 0 && c.get(u) == DFS_WHITE) {
            ret.emplace_back(UserCall(UserCall::Type::preprocess, u));
            c.insert(u, DFS_GRAY);
        }
        if (k < g.deg(u)) {
            sj->push(std::pair<uint64_t, uint64_t>(u, k + 1));
            uint64_t v = g.head(u, k);
            if (c.get(v) == DFS_WHITE) {
                ret.emplace_back(UserCall(UserCall::Type::preexplore, u, k));
                sj->push(std::pair<uint64_t, uint64_t>(v, 0));
            } else {
                // no postexplore: will be simulated later
            }
        } else {
            c.insert(u, DFS_BLACK);
            ret.emplace_back(UserCall(UserCall::Type::postprocess, u));
            if (sj->size() > 0) {
                uint64_t pu = sj->top().first, pk = sj->top().second;
                ret.emplace_back(
                    UserCall(UserCall::Type::postexplore, pu,
                             pk - 1));  // this postexplore is a sequence call
            }
        }
        if (sj->empty()) {
            bool a = false;
            for (uint64_t b = 0; b < n; b++) {
                if (c.get(b) == DFS_WHITE && d.get(b) == ip) {
                    sj->push(std::pair<uint64_t, uint64_t>(b, 0));
                    a = true;
                    break;
                } else if (c.get(b) == DFS_GRAY && f.get(b) == ip) {
                    sj->push(std::pair<uint64_t, uint64_t>(b, g.deg(b)));
                    a = true;
                    break;
                }
            }
            if (!a) break;
        }
    }
    return ret;
}*/

/*void ReverseDFS::process_recording(uint64_t u0) {
    updateInterval(0);
    s.push(std::pair<uint64_t, uint64_t>(u0, 0));
    niveau = 1;
    std::pair<uint64_t, uint64_t> x;
    while (!s.isEmpty()) {
        uint64_t actions = 0;
        int sr = s.pop(&x);
        if (sr == DFS_DO_RESTORE) {
            restore_top(u0, g, &c, &s);
            s.pop(&x);
        } else if (sr == DFS_NO_MORE_NODES) {
            return;
        }
        uint64_t u = x.first, k = x.second;
        if (k == 0 && c.get(u) == DFS_WHITE) {
            setCall({UserCall::preprocess, u});
            storeTime(0, u);
            c.insert(u, DFS_GRAY);
            actions++;
        }
        if (k < g.deg(u)) {
            s.push(std::pair<uint64_t, uint64_t>(u, k + 1));
            uint64_t v = g.head(u, k);
            setCall({UserCall::preexplore, u, k});
            if (c.get(v) == DFS_WHITE) {
                s.push(std::pair<uint64_t, uint64_t>(v, 0));
                actions++;
                niveau++;
            }
            // (postexplore can never be a first call in an interval)
        } else {
            c.insert(u, DFS_BLACK);
            storeTime(1, u);
            setCall({UserCall::postprocess, u});
            actions++;
            niveau--;
        }
        updateInterval(actions);
    }
}

void ReverseDFS::storeTime(bool df, uint64_t u) {
    if (df == 0) {
        if (d.get(u) == iCount + 1) {
            d.insert(u, ip);
        }
    } else if (df == 1) {
        if (f.get(u) == iCount + 1) {
            f.insert(u, ip);
        }
    }
}

void ReverseDFS::updateInterval(uint64_t actions, bool end) {
    IntervalData &ci = intervals[ip];
    std::pair<uint64_t, uint64_t> top = s.top();
    if (ci.width == 0 && ci.inUse == false) {
        ci.top = top;
        ci.inUse = true;
    }
    ci.width += actions;
    if (niveau < ci.depth && s.top().first != INVALID) {
        ci.bottom = top;
        ci.depth = niveau;
    }
    if (ci.width > iWidth || end) {
        ci.top2 = top;
        ci.firstCall = fCall;
        fCall = UserCall();
        if (ip >= iCount) throw std::out_of_range("too many intervals");
        updateInterval(0);
    }
}

void ReverseDFS::setCall(UserCall call) {
    if (fCall == UserCall()) {
        fCall = call;
    }
}*/

}  // namespace Sealib
