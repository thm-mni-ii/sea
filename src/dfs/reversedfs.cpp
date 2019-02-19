#include "sealib/iterator/reversedfs.h"
#include <cmath>
#include <set>

namespace Sealib {

ReverseDFS::ReverseDFS(Graph const &graph)
    : g(graph),
      n(g.getOrder()),
      iCount(static_cast<uint64_t>(20 * WORD_SIZE / log2(WORD_SIZE)) + 1),
      iWidth(static_cast<uint64_t>(n * log2(WORD_SIZE) / WORD_SIZE) + 1),
      c(n, 3),
      d(n, iCount),
      f(n, iCount),
      s(n, g, &c),
      intervals(iCount),
      sequence(),
      seqI(sequence.rend()) {
    for (uint64_t a = 0; a < n; a++) {
        c.insert(a, 0);
        // d.insert(a, iCount + 1);
        // f.insert(a, iCount + 1);
    }
}

void ReverseDFS::init() {
    i->top1 = i->bottom = {INVALID, INVALID};
    i->call1 = UserCall(UserCall::preprocess, 0);
    for (uint64_t u = 0; u < n; u++) {
        if (c.get(u) == DFS_WHITE) {
            DFS::visit_nloglogn(
                u, g, &c, &s, [&](uint64_t u0) { restore_top(u0, g, &c, &s); },
                [this](uint u) {
                    if (i->width > iWidth) {
                        advanceInterval();
                        // we peek at the stack because of preceding (u,k)<=S
                        std::pair<uint64_t, uint64_t> p;
                        uint8_t r = s.pop(&p);
                        if (r == DFS_NO_MORE_NODES) {
                            i->top1 = i->bottom = {INVALID, INVALID};
                        } else if (r == DFS_DO_RESTORE) {
                            // ?
                        } else {
                            s.push(p);
                            i->top1 = i->bottom = p;
                        }
                        i->call1 = UserCall(UserCall::preprocess, u);
                    }
                    d.insert(u, ip);
                    i->width++;
                },
                [this](uint64_t u, uint64_t k) {
                    uint64_t v = g.head(u, k);
                    if (i->width > iWidth) {
                        advanceInterval();
                        i->top1 = i->bottom = {u, k + 1};
                        i->call1 = UserCall(UserCall::preexplore, u, k);
                    } else if (i->width == iWidth && c.get(v) == DFS_WHITE) {
                        advanceInterval();
                        i->top1 = i->bottom = {v, 0};
                        i->call1 = UserCall(UserCall::preprocess, v);
                    }
                    i->width++;
                },
                [this](uint64_t u, uint16_t k) {
                    // this should only be executed in the meaningful
                    // postexplore (major call)
                    if (i->width > iWidth) {
                        advanceInterval();
                        // we peek at the stack because of preceding (pu,pk)<=S
                        std::pair<uint64_t, uint64_t> p;
                        s.pop(&p);
                        s.push(p);
                        i->top1 = i->bottom = p;
                        i->call1 = UserCall(UserCall::postexplore, u, k);
                    } else if (i->width == iWidth) {
                        advanceInterval();
                        // we know that S<=(u,k+1) is the next instruction
                        i->top1 = i->bottom = {u, k + 1};
                        // the only next possible user call since (u,k+1) will
                        // be at top
                        i->call1 = UserCall(UserCall::preexplore, u, k + 1);
                    }
                    // peek the stack to see in which postexplore() we are
                    std::pair<uint64_t, uint64_t> p;
                    uint8_t a = s.pop(&p);
                    if (a == 0) s.push(p);
                    // if pop failed, we must also be in the major postexplore
                    if (a != 0 || p.first != u) {
                        // this means we have a preceding (u,k+1)<=S
                        // -> "major" postexplore
                        i->width++;
                    }
                },
                [this](uint u) { f.insert(u, ip); });
        }
    }
    assert(ip < iCount);
}

void ReverseDFS::advanceInterval() {
    ip++;
    i++;
}

UserCall ReverseDFS::next() {
    if (/*seqI != sequence.rend()*/ false) {
        static uint64_t mu = INVALID, mk = 0;
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
        return ret;
    } else {  // build new sequence
        if (i > intervals.begin())
            i--;
        else
            return UserCall();
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
        while (a != i->top1) {
            for (uint64_t b = 0; /*crashes if not found*/; b++) {
                uint64_t v = g.head(a.first, b);
                if (f.get(v) == ip && d.get(v) < ip && !used[v]) {
                    used[v] = true;
                    r.top().second = b + 1;
                    a = {v, INVALID};
                    break;
                }
            }
            s.push(a);
        }  // loop only ends when top has been pushed
        r.top() = i->top1;
    }
    return r;
}

/*std::stack<std::pair<uint64_t, uint64_t>> ReverseDFS::reconstructStack() {
    std::stack<std::pair<uint64_t, uint64_t>> sj;
    std::set<uint64_t> tmp;
    std::pair<uint64_t, uint64_t> &to = i->top1, &from = i->bottom;
    if (to.first != INVALID) {
        if (from.first == INVALID) {
            sj.push(to);
        } else {
            std::pair<uint64_t, uint64_t> a =
                std::pair<uint64_t, uint64_t>(from.first, 0);
            tmp.insert(a.first);
            do {
                uint64_t u = a.first;
                if (u == to.first) {
                    sj.push(to);
                } else {
                    bool found = false;
                    for (uint64_t k = a.second; k < g.deg(u); k++) {
                        uint64_t v = g.head(u, k);
                        if (f.get(v) == ip && tmp.find(v) == tmp.end()) {
                            // using a temp. set to avoid cycles
                            a = std::pair<uint64_t, uint64_t>(v, 0);
                            sj.push(std::pair<uint64_t, uint64_t>(u, k + 1));
                            tmp.insert(v);
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        throw std::logic_error(
                            "no white path found in reconstruction");
                    }
                }
            } while (sj.top() != to);
        }
    }
    return sj;
}*/

std::vector<UserCall> ReverseDFS::simulate(
    std::stack<std::pair<uint64_t, uint64_t>> *sj) {
    std::vector<UserCall> r;
    if (sj->empty()) {
        if (i->call1.type == UserCall::preprocess) {
            sj->push({i->call1.u, 0});
        } else {
            throw IntervalStackEmpty();
        }
    }

    while (!sj->empty() && sj->top() != (i - 1)->top1) {
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
            } else {
                r.emplace_back(UserCall(UserCall::postexplore, u, k));
            }
        } else {
            c.insert(u, DFS_BLACK);
            r.emplace_back(UserCall(UserCall::Type::postprocess, u));
            if (sj->size() > 0) {
                std::pair<uint64_t, uint64_t> p = sj->top();
                r.emplace_back(UserCall(UserCall::Type::postexplore, p.first,
                                        p.second - 1));
            }
        }
    }
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

void ReverseDFS::process_recording(uint64_t u0) {
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
        ci.top1 = top;
        ci.inUse = true;
    }
    ci.width += actions;
    if (niveau < ci.depth && s.top().first != INVALID) {
        ci.bottom = top;
        ci.depth = niveau;
    }
    if (ci.width > iWidth || end) {
        ci.top2 = top;
        ci.call1 = firstCall;
        firstCall = UserCall();
        if (ip >= iCount) throw std::out_of_range("too many intervals");
        updateInterval(0);
    }
}

void ReverseDFS::setCall(UserCall call) {
    if (firstCall == UserCall()) {
        firstCall = call;
    }
}

}  // namespace Sealib
