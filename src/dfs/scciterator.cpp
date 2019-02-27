#include "sealib/iterator/scciterator.h"
#include "sealib/graph/graphcreator.h"

namespace Sealib {

SCCIterator::SCCIterator(DirectedGraph const &graph, Consumer p1, BiConsumer p2,
                         BiConsumer p3, Consumer p4)
    : g(graph),
      n(g.getOrder()),
      t(GraphCreator::transpose(g)),
      d(g),
      c(n, 3),
      s(n, g, &c),
      preprocess(p1),
      preexplore(p2),
      postexplore(p3),
      postprocess(p4) {
    for (uint64_t u = 0; u < n; u++) {
        c.insert(u, 0);
    }
}

void SCCIterator::init() { d.init(); }

bool SCCIterator::more() {
    uint64_t u;
    while ((u = nextRoot()) != INVALID) {
        if (c.get(u) == DFS_WHITE) {
            u0 = u;
            return true;
        }
    }
    return false;
}

uint64_t SCCIterator::next() {
    DFS::visit_nloglogn(u0, g, &c, &s, DFS::restore_top, preprocess, preexplore,
                        postexplore, postprocess);
    return u0;
}

uint64_t SCCIterator::nextRoot() {
    while (d.more()) {
        UserCall a = d.next();
        if (a.type == UserCall::postprocess) {
            return a.u;
        }
    }
    return INVALID;
}

}  // namespace Sealib