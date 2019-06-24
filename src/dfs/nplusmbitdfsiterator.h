#ifndef SRC_DFS_NPLUSMBITDFSITERATOR_H_
#define SRC_DFS_NPLUSMBITDFSITERATOR_H_

#include "sealib/iterator/iterator.h"

namespace Sealib {

class NplusMBitDFSIterator : public Iterator<UserCall> {
 public:
    explicit NplusMBitDFSIterator(UndirectedGraph const &graph, uint64_t u0)
        : g(graph),
          root(u0),
          state(0),
          nextposRoot(0),
          color(g.getOrder(), 3),
          parent(g),
          finished(false) {}

    void init() override {}

    bool more() override { return !finished; }

    UserCall next() override {
        if (state == 0) {
            state = 1;
            color.insert(root, DFS_GRAY);
            r.u = root, r.k = 0;
            r.type = UserCall::preprocess;
            return r;
        }
        if (state != 5) {
            if (r.k < g.deg(r.u)) {
                if (state == 1) {
                    state = 2;
                    v = g.head(r.u, r.k);
                    r.type = UserCall::preexplore;
                    return r;
                }
                if (color.get(v) == DFS_WHITE) {
                    if (state == 2) {
                        state = 3;
                        UserCall vcall;
                        vcall.type = UserCall::preprocess;
                        vcall.u = v;
                        return vcall;
                    }
                    if (color.get(v) == DFS_BLACK) {
                        state = 5;
                        return next();
                    }
                    color.insert(v, DFS_GRAY);
                    parent.insert(v, g.mate(r.u, r.k));
                    r.u = v;
                    r.k = 0;
                    state = 1;
                } else {
                    if (state == 2) {
                        state = 3;
                        r.type = UserCall::postexplore;
                        return r;
                    }
                    r.k++;
                    state = 1;
                }
            } else {
                if (state == 1) {
                    state = 2;
                    color.insert(r.u, DFS_BLACK);
                    r.type = UserCall::postprocess;
                    return r;
                }
                if (r.u != root) {
                    if (state == 2) {
                        state = 3;
                        uint64_t pk = g.mate(r.u, parent.get(r.u));
                        uint64_t pu = g.head(r.u, parent.get(r.u));
                        r.type = UserCall::postexplore;
                        r.u = pu;
                        r.k = pk;
                        return r;
                    }
                    state = 1;
                    r.k++;
                } else {
                    state = 5;
                }
            }
            return next();
        }
        for (; nextposRoot < g.getOrder(); nextposRoot++) {
            if (color.get(nextposRoot) == DFS_WHITE) {
                root = nextposRoot;
                state = 0;
                return next();
            }
        }
        finished = true;
        r.type = UserCall::nop;
        return r;
    }

 private:
    UndirectedGraph const &g;
    uint64_t root;
    uint64_t state;
    uint64_t nextposRoot;
    uint64_t v;
    CompactArray color;
    StaticSpaceStorage parent;
    UserCall r;
    bool finished;
};

}  // namespace Sealib

#endif  // SRC_DFS_NPLUSMBITDFSITERATOR_H_
