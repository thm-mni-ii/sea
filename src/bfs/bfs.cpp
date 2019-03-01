#include "sealib/iterator/bfs.h"
#include <utility>
#include <vector>

namespace Sealib {

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
    for (uint64_t a = u; a < n; a++) {
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
    } catch (ChoiceDictionaryEmpty) {
        try {
            isOuter.choice();
        } catch (ChoiceDictionaryEmpty) {
            return false;
        }
    }
    return true;
}
uint64_t BFS::getGrayNode() {
    uint64_t r = INVALID;
    try {
        r = isInner.choice();
    } catch (ChoiceDictionaryEmpty) {
        try {
            r = isOuter.choice();
        } catch (ChoiceDictionaryEmpty) {
            throw NoMoreGrayNodes();
        }
    }
    return static_cast<uint64_t>(r);
}

bool BFS::more() { return hasGrayNode(); }

std::pair<uint64_t, uint64_t> BFS::next() {
    u = getGrayNode();
    if (color.get(u) == outerGray) {
        uint32_t tmp = innerGray;
        innerGray = outerGray;
        outerGray = tmp;
        std::swap(isInner, isOuter);
        dist++;
    }
    for (uint64_t k = 0; k < g.deg(u); k++) {
        uint64_t v = g.head(u, k);
        preexplore(u, v);
        if (color.get(v) == BFS_WHITE) {
            preprocess(v);
            color.insert(v, outerGray);
            isOuter.insert(v);
        }
    }
    isInner.remove(u);
    color.insert(u, BFS_BLACK);
    return std::pair<uint64_t, uint64_t>(u, dist);
}

BFS::BFS(Graph const &graph, Consumer pp, BiConsumer pe)
    : BFS(graph, CompactArray(graph.getOrder(), 4), pp, pe) {}

BFS::BFS(Graph const &graph, CompactArray c, Consumer pp, BiConsumer pe)
    : g(graph),
      n(g.getOrder()),
      color(std::move(c)),
      isInner(n),
      isOuter(n),
      preprocess(pp),
      preexplore(pe) {}

}  // namespace Sealib
