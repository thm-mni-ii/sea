#ifndef SRC_DFS_STANDARDDFSITERATOR_H_
#define SRC_DFS_STANDARDDFSITERATOR_H_

#include "sealib/iterator/iterator.h"

namespace Sealib {

class StandardDFSIterator : public Iterator<UserCall> {
 public:
    explicit StandardDFSIterator(Graph const &graph, uint64_t u0)
        : g(graph),
          root(u0),
          state(0),
          nextposRoot(0),
          color(g.getOrder()),
          finished(false) {
        s.push_back({root, 0});
    }

    void init() override {}

    bool more() override { return !finished; }

    UserCall next() override {
        if (!s.empty() || state != 0) {
            if (state == 0) {
                state = 1;
                std::pair<uint64_t, uint64_t> x = s.back();
                s.pop_back();
                r.u = x.first;
                r.k = x.second;
                if (color.operator[](r.u) == DFS_WHITE) {
                    color.operator[](r.u) = DFS_GRAY;
                    r.type = UserCall::preprocess;
                    return r;
                }
            }
            if (r.k < g.deg(r.u)) {
                if (state == 1) {
                    state = 2;
                    s.push_back({r.u, r.k + 1});
                    v = g.head(r.u, r.k);
                    r.type = UserCall::preexplore;
                    return r;
                } else {
                    if (color.operator[](v) == DFS_WHITE) {
                        s.push_back({v, 0});
                    } else {
                        state = 0;
                        r.type = UserCall::postexplore;
                        return r;
                    }
                }
            } else {
                if (state == 1) {
                    state = 2;
                    color.operator[](r.u) = DFS_BLACK;
                    r.type = UserCall::postprocess;
                    return r;
                }
                if (r.u != root) {
                    state = 0;
                    std::pair<uint64_t, uint64_t> p = s.back();
                    r.type = UserCall::postexplore;
                    r.u = p.first;
                    r.k = p.second - 1;
                    return r;
                }
            }
            state = 0;
            return next();
        }
        for (; nextposRoot < g.getOrder(); nextposRoot++) {
            if (color[nextposRoot] == DFS_WHITE) {
                root = nextposRoot;
                s.push_back({root, 0});
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
    std::vector<uint64_t> color;
    std::vector<std::pair<uint64_t, uint64_t>> s;
    UserCall r;
    bool finished;
};

}  // namespace Sealib

#endif  // SRC_DFS_STANDARDDFSITERATOR_H_
