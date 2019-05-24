#include "./simplebfs.h"

namespace Sealib {

SimpleBFS::SimpleBFS(Graph const &graph, Consumer pp, BiConsumer pe)
    : g(graph), n(g.getOrder()), color(n), preprocess(pp), preexplore(pe) {
    for (uint64_t a = 0; a < n; a++) {
        color[a] = BFS_WHITE;
    }
}

void SimpleBFS::init() {
    color[0] = BFS_GRAY1;
    queue.push({0, 0});
}

bool SimpleBFS::more() { return !queue.empty(); }

bool SimpleBFS::nextComponent() {
    for (uint64_t u = 0; u < n; u++) {
        if (color[u] == BFS_WHITE) {
            queue.push({u, 0});
            return true;
        }
    }
    return false;
}

std::pair<uint64_t, uint64_t> SimpleBFS::next() {
    std::pair<uint64_t, uint64_t> p = queue.front();
    queue.pop();
    uint64_t u = p.first, d = p.second;
    preprocess(u);
    for (uint64_t k = 0; k < g.deg(u); k++) {
        uint64_t v = g.head(u, k);
        preexplore(u, v);
        if (color[v] == BFS_WHITE) {
            color[v] = BFS_GRAY1;
            queue.push({v, d + 1});
        }
    }
    if (queue.size() > qmax) qmax = queue.size();
    color[u] = BFS_BLACK;
    return {u, d};
}

}  // namespace Sealib
