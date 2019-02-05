#include "sealib/iterator/bfs.h"
#include <stdexcept>
#include <utility>
#include <vector>

namespace Sealib {

class NoMoreGrayNodes : std::exception {
    const char *what() const noexcept {
        return "BFS: no more gray nodes found; did you forget to call "
               "nextComponent()?";
    }
};

void BFS::init() {
    u = 0;
    dist = 0;
    innerGray = BFS_GRAY1;
    outerGray = BFS_GRAY2;
    preprocess(0);
    color.insert(0, innerGray);
    isInner.insert(0);
}

bool BFS::nextComponent() {
    bool found = false;
    for (uint a = 0; a < n; a++) {
        if (color.get(a) == BFS_WHITE) {
            u = a;
            found = true;
            dist = 0;
            preprocess(u);
            color.insert(u, innerGray);
            isInner.insert(u);
            break;
        }
    }
    return found;
}

bool BFS::hasGrayNode() {
    try {
        isInner.choice();
    } catch (std::exception e) {
        try {
            isOuter.choice();
        } catch (std::exception e2) {
            return false;
        }
    }
    return true;
}
uint BFS::getGrayNode() {
    uint r = INVALID;
    try {
        r = isInner.choice();
    } catch (std::exception e) {
        try {
            r = isOuter.choice();
        } catch (std::exception e2) {
            throw NoMoreGrayNodes();
        }
    }
    return r;
}

bool BFS::more() { return hasGrayNode(); }

std::pair<uint, uint> BFS::next() {
    u = getGrayNode();
    if (color.get(u) == outerGray) {
        uint32_t tmp = innerGray;
        innerGray = outerGray;
        outerGray = tmp;
        std::swap(isInner, isOuter);
        dist++;
    }
    for (uint k = 0; k < g->deg(u); k++) {
        uint v = g->head(u, k);
        preexplore(u, v);
        if (color.get(v) == BFS_WHITE) {
            preprocess(v);
            color.insert(v, outerGray);
            isOuter.insert(v);
        }
    }
    isInner.remove(u);
    color.insert(u, BFS_BLACK);
    return std::pair<uint, uint>(u, dist);
}

void BFS::forEach(std::function<void(std::pair<uint, uint>)> f) {
    do {
        while (more()) f(next());
    } while (nextComponent());
}

BFS::BFS(Graph const *graph, Consumer pp, BiConsumer pe)
    : g(graph),
      n(g->getOrder()),
      color(n, 4),
      isInner(n),
      isOuter(n),
      preprocess(pp),
      preexplore(pe) {
    for (uint a = 0; a < n; a++) color.insert(a, BFS_WHITE);
}

BFS::BFS(Graph const *graph, CompactArray c, Consumer pp, BiConsumer pe)
    : g(graph),
      n(g->getOrder()),
      color(std::move(c)),
      isInner(n),
      isOuter(n),
      preprocess(pp),
      preexplore(pe) {
    for (uint a = 0; a < n; a++) color.insert(a, BFS_WHITE);
}

}  // namespace Sealib
