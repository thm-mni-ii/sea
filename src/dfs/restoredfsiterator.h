#ifndef SRC_DFS_RESTOREDFSITERATOR_H_
#define SRC_DFS_RESTOREDFSITERATOR_H_

#include "sealib/iterator/iterator.h"

namespace Sealib {

class RestoreDFSIterator : public Iterator<UserCall> {
 public:
    explicit RestoreDFSIterator(
        Graph const &graph, uint64_t u0,
        std::function<void(uint64_t, Graph const &, CompactArray *,
                           SegmentStack *)>
            rest,
        bool nBit)
        : g(graph),
          root(u0),
          state(0),
          nextposRoot(0),
          restore(std::move(rest)),
          color(g.getOrder(), 3),
          finished(false) {
        if (nBit) {
            uint64_t n = g.getOrder();
            uint64_t q = static_cast<uint64_t>(
                ceil(ceil(0.2f / 6 * n) /
                     (8 * sizeof(std::pair<uint64_t, uint64_t>))));
            uint64_t qs = 3;
            if (q < qs) q = qs;
            s = new BasicSegmentStack(q);
        } else {
            s = new ExtendedSegmentStack(g.getOrder(), g, &color);
        }
        s->push({u0, 0});
    }

    void init() override {}

    bool more() override { return !finished; }

    UserCall next() override {
        if (!s->isEmpty() || state != 0) {
            if (state == 0) {
                state = 1;
                sr = s->pop(&x);
                if (sr == DFS_DO_RESTORE) {
                    restore(root, g, &color, s);
                    s->pop(&x);
                } else if (sr == DFS_NO_MORE_NODES) {
                    state = 0;
                    return next();
                }
                r.u = x.first;
                r.k = x.second;
                if (color.get(r.u) == DFS_WHITE) {
                    color.insert(r.u, DFS_GRAY);
                    r.type = UserCall::preprocess;
                    return r;
                }
            }
            if (r.k < g.deg(r.u)) {
                if (state == 1) {
                    state = 2;
                    s->push({r.u, r.k + 1});
                    v = g.head(r.u, r.k);
                    r.type = UserCall::preexplore;
                    return r;
                }
                state = 0;
                if (color.get(v) == DFS_WHITE) {
                    s->push({v, 0});
                    return next();
                } else {
                    r.type = UserCall::postexplore;
                    return r;
                }
            } else {
                if (state == 1) {
                    state = 2;
                    color.insert(r.u, DFS_BLACK);
                    r.type = UserCall::postprocess;
                    return r;
                }
                if (r.u != root) {
                    std::pair<uint64_t, uint64_t> px;
                    sr = s->pop(&px);
                    if (sr == DFS_DO_RESTORE) {
                        restore(root, g, &color, s);
                        s->pop(&px);
                    }
                    s->push(px);
                    r.type = UserCall::postexplore;
                    r.u = px.first;
                    r.k = px.second - 1;
                    state = 0;
                    return r;
                }
            }
            state = 0;
            return next();
        }
        for (; nextposRoot < g.getOrder(); nextposRoot++) {
            if (color.get(nextposRoot) == DFS_WHITE) {
                root = nextposRoot;
                s->push({root, 0});
                state = 0;
                return next();
            }
        }
        finished = true;
        r.type = UserCall::nop;
        return r;
    }

 private:
    Graph const &g;
    uint64_t root;
    uint64_t state;
    uint64_t nextposRoot;
    uint64_t v;
    std::function<void(uint64_t, Graph const &, CompactArray *, SegmentStack *)>
        restore;
    std::pair<uint64_t, uint64_t> x;
    SegmentStack *s;
    CompactArray color;
    UserCall r;
    uint8_t sr;
    bool finished;
};

}  // namespace Sealib

#endif  // SRC_DFS_RESTOREDFSITERATOR_H_
